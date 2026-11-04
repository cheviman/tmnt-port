/**
 *    Name:        game_program.c
 *    Description: the decompiled 68000 program (src/, tools/decompile.py) as a port runs it: its reset, its run
 *                 on a stack of its own, the frames, the status register and the interrupts, and the tables it
 *                 reads by name, loaded from the assets
 *    Version:     0.1.0
 *    Created:     29-10-2026
 *    Modified:    29-10-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The program runs with frame timing, as the translation scaffold does (runtime/src/m68000.c) and kref does by
 *    default (tmnt/frame.h): instructions take no time, and the board moves on to the start of its next VBLANK
 *    where the main loop finds nothing to do (frame_wait). The interrupts it raised are taken there and after a
 *    write of the status register (take_interrupts), as MAME's core takes them between two instructions: the
 *    handler of the level's autovector runs, in supervisor mode with the mask at its level. The VBLANK handler
 *    goes back to the code it interrupted in user mode with RTR, which leaves the status register as the handler
 *    left it; RTE (return_from_exception) puts back the one the interrupt found.
 *
 *    The tables the C reads by name (src/table.c's registry) are the assets' pieces: the loader copies each one
 *    from its address on, as far as the pieces run without a gap, with every element turned to the host's byte
 *    order. Nothing of the ROM is read: the data is the assets' (game/assets.h).
 */
#include "game/support.h"

#include "host/tasks.h"
#include "tmnt/frame.h"
#include "tmnt/machine.h"
#include "tmnt/program.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SR_MASK      0xA700u    /* the system byte: trace, supervisor, the interrupt mask */
#define SR_S         0x2000u
#define MAX_DEPTH    16
#define MAX_TABLE    0x10000u   /* the bytes of a table's copy, at most */

void (*program_vblank_end)(void);
void (*program_vblank_start)(void);
void (*program_frame_done)(void);

static uint32_t status = 0x2700u;           /* the status register's system byte, as the reset leaves it */
static uint32_t saved[MAX_DEPTH];           /* what each interrupt being handled found */
static size_t depth;
static uint32_t interrupts;
static host_task *port_task;
static bool tables_loaded;

/* the last functions and blocks that ran (tools/decompile.py --trace), shown when the program stops */
#define TRACE_SIZE 48
static uint32_t traced[TRACE_SIZE];
static unsigned traced_count;

void game_trace(uint32_t address)
{
	traced[traced_count++ % TRACE_SIZE] = address;
}

void game_trace_block(uint32_t address)
{
	traced[traced_count++ % TRACE_SIZE] = address | 0x80000000u;
}

static void show_trace(void)
{
	if (!traced_count)
		return;
	fprintf(stderr, "the last functions (F) and blocks (B) that ran:");
	for (unsigned index = traced_count > TRACE_SIZE ? traced_count - TRACE_SIZE : 0; index < traced_count; index++)
		fprintf(stderr, " %c%06X", traced[index % TRACE_SIZE] & 0x80000000u ? 'B' : 'F',
			(unsigned)traced[index % TRACE_SIZE] & 0xFFFFFFu);
	fprintf(stderr, "\n");
}

/* With TMNT_WATCH=ADDRESS (hexadecimal) in the environment, a traced build stops at the first write the program
   makes there on the bus from frame TMNT_WATCH_FRAME on, and shows the trace (a development aid) */
static uint32_t watched = 0xFFFFFFFFu;
static uint64_t watched_from;

static void show_trace(void);

static void watch_write(uint32_t address, uint16_t data, uint16_t mem_mask)
{
	if (address == (watched & 0xFFFFFEu) && tmnt_board.frame >= watched_from
		&& (mem_mask & ((watched & 1u) ? 0x00FFu : 0xFF00u)))
	{
		show_trace();
		fprintf(stderr, "write %04X (mask %04X) at %06X (frame %llu)\n", data, mem_mask, (unsigned)address,
			(unsigned long long)tmnt_board.frame);
		if (!getenv("TMNT_WATCH_ALL"))
			exit(4);
		traced_count = 0;
	}
}

/* The program read its own code as data, which the game does not have */
static void code_read(uint32_t address)
{
	show_trace();
	fprintf(stderr, "68000 stopped: the program read its code at %06X as data (frame %llu)\n",
		(unsigned)address & 0xFFFFFFu, (unsigned long long)tmnt_board.frame);
	exit(3);
}

/* ---- the program's data ---- */

const uint8_t *data_at(uint32_t address)
{
	const uint8_t *data = assets_data_at(address & 0xFFFFFFu);

	if (data)
		return data;
	if ((address & 0xFFFFFFu) >= TMNT_WORK_RAM && (address & 0xFFFFFFu) < TMNT_WORK_RAM + TMNT_WORK_RAM_SIZE)
		return tmnt_work_ram_byte(address & 0xFFFFFFu);    /* (a pointer the program set to the work RAM) */
	show_trace();
	fprintf(stderr, "68000 stopped: the program read data at %06X, where its assets have none (frame %llu)\n",
		(unsigned)address & 0xFFFFFFu, (unsigned long long)tmnt_board.frame);
	exit(3);
}

/* A record of the program's data, its bytes the other way round (game/support.h): in the reversed copy of the
   piece it is in; one across two pieces, from their bytes into one of a few copies (a record is read where the
   C names it: no pointer to one outlives its expression). A record the code reads only in part may run past the
   pieces: an animation frame of 10 bytes read as an attack_frame (18), whose attack box the code reads only once
   its attack_depth says it has one. Those bytes are no data of the program's, and read 0 */
#define RECORD_COPIES 32
#define RECORD_MAX    64u

const void *data_record(uint32_t address, uint32_t size)
{
	static uint8_t copies[RECORD_COPIES][RECORD_MAX];
	static unsigned next;
	const void *found;
	uint8_t *copy;

	address &= 0xFFFFFFu;
	if (address >= TMNT_WORK_RAM && address + size <= TMNT_WORK_RAM + TMNT_WORK_RAM_SIZE)
		return tmnt_work_ram_at(address, size);    /* (a pointer the program set to the work RAM) */
	if ((found = assets_record_at(address, size)) != NULL)
		return found;
	if (size > RECORD_MAX)
	{
		show_trace();
		fprintf(stderr, "68000 stopped: the program read a record of %u bytes at %06X across its assets' pieces "
			"(frame %llu)\n", (unsigned)size, (unsigned)address, (unsigned long long)tmnt_board.frame);
		exit(3);
	}
	copy = copies[next++ % RECORD_COPIES];
	for (uint32_t at = 0; at < size; at++)
	{
		const uint8_t *byte = assets_data_at(address + at);

		copy[size - 1u - at] = byte ? *byte : 0u;
	}
	return copy;
}

/* Every table of the registry: a copy of the data from its address on, as long as the pieces run, each element
   in the host's byte order. NULL, or what failed */
static const char *load_tables(void)
{
	static char problem[160];

	for (size_t index = 0; index < game_table_count; index++)
	{
		const game_table *table = &game_tables[index];
		uint32_t length = 0;
		uint8_t *copy;

		while (length < MAX_TABLE && assets_data_at(table->address + length))
			length++;
		if (!length)
		{
			snprintf(problem, sizeof(problem), "the program's table %s is in no asset", table->name);
			return problem;
		}
		copy = malloc(length + 4u);
		if (!copy)
		{
			snprintf(problem, sizeof(problem), "no memory for the program's table %s", table->name);
			return problem;
		}
		memset(copy, 0, length + 4u);
		for (uint32_t offset = 0; offset < length; offset++)
			copy[offset] = *assets_data_at(table->address + offset);
		/* (big endian to the host's: each element's bytes the other way round) */
		if (table->element > 1)
			for (uint32_t first = 0; first + table->element <= length; first += table->element)
				for (uint32_t low = first, high = first + table->element - 1u; low < high; low++, high--)
				{
					const uint8_t byte = copy[low];

					copy[low] = copy[high];
					copy[high] = byte;
				}
		*table->data = copy;
	}
	return NULL;
}

/* ---- the code the board runs by address ---- */

static void (*code_at(uint32_t address))(void)
{
	size_t low = 0, high = game_code_count;

	address &= 0xFFFFFFu;
	while (low < high)
	{
		const size_t middle = (low + high) / 2;

		if (game_code[middle].address < address)
			low = middle + 1;
		else
			high = middle;
	}
	if (low < game_code_count && game_code[low].address == address)
		return game_code[low].function;
	fprintf(stderr, "68000 stopped: no code of the program starts at %06X (frame %llu)\n", (unsigned)address,
		(unsigned long long)tmnt_board.frame);
	exit(3);
}

void invalid_jump(uint32_t address)
{
	fprintf(stderr, "68000 stopped: a jump to %06X, which no case of its switch has (frame %llu)\n",
		(unsigned)address & 0xFFFFFFu, (unsigned long long)tmnt_board.frame);
	exit(3);
}

void lost_stack(uint32_t address)
{
	fprintf(stderr, "68000 stopped: the code at %06X keeps a stack the C does not have (frame %llu)\n",
		(unsigned)address & 0xFFFFFFu, (unsigned long long)tmnt_board.frame);
	exit(3);
}

/* ---- the status register and the interrupts ---- */

void write_status(uint32_t value)
{
	status = value & SR_MASK;
}

void return_from_exception(void)
{
	if (depth)
		status = saved[depth - 1];
}

/* An interrupt's handler, with what the code carries in its registers (game_context) put back when it returns: the
   VBLANK handler saves D0-D7/A0-A6 (003DC4 "movem.l D0-D7/A0-A6, $60440.l") and puts them back before it goes back
   to the code it interrupted (003E44 "movem.l $60440.l, D0-D7/A0-A6"), which finds its registers as it left them */
#define MAX_CONTEXT 16

static void run_handler(void (*handler)(void))
{
	uint32_t kept[MAX_CONTEXT];

	if (game_context_count > MAX_CONTEXT)
	{
		fprintf(stderr, "68000 stopped: the program carries more values in its registers than the runtime keeps\n");
		exit(3);
	}
	for (size_t index = 0; index < game_context_count; index++)
		kept[index] = *game_context[index];
	handler();
	for (size_t index = 0; index < game_context_count; index++)
		*game_context[index] = kept[index];
}

void take_interrupts(void)
{
	for (;;)
	{
		const int level = tmnt_irq_level();
		void (*handler)(void);

		if (level == 0 || (uint32_t)level <= (status >> 8 & 7u))
			return;
		if (depth == MAX_DEPTH)
		{
			fprintf(stderr, "68000 stopped: more than %d interrupts nest (frame %llu)\n", MAX_DEPTH,
				(unsigned long long)tmnt_board.frame);
			exit(3);
		}
		handler = code_at(data32(4u * (24u + (uint32_t)level)));
		saved[depth++] = status;
		status = (status & ~0x8700u) | SR_S | (uint32_t)level << 8;
		interrupts++;
		run_handler(handler);
		depth--;
	}
}

/* ---- the frame ---- */

/* The board moves on to the start of its next VBLANK, line 240, past the end of the last one, line 16 (where
   the video takes the scroll); the frame is drawn when VBLANK starts, before the board's events */
static void next_frame(void)
{
	const uint64_t start = tmnt_board.frame * TMNT_FRAME_CYCLES;

	tmnt_board.cycles = start + (uint64_t)TMNT_VBLANK_END * TMNT_LINE_CYCLES;
	if (program_vblank_end)
		program_vblank_end();
	tmnt_board.cycles = start + (uint64_t)TMNT_VBLANK_START * TMNT_LINE_CYCLES;
	if (program_vblank_start)
		program_vblank_start();
	tmnt_vblank_start();
	if (program_frame_done)
		program_frame_done();
}

void frame_wait(void)
{
	if (tmnt_frame_idle())
		next_frame();
}

void stop(uint32_t value)
{
	write_status(value);
	while (tmnt_irq_level() == 0 || (uint32_t)tmnt_irq_level() <= (status >> 8 & 7u))
		next_frame();
	take_interrupts();
}

void reset_devices(void)
{
	/* (the board connects nothing to the 68000's RESET line) */
}

/* ---- the program as a port runs it (tmnt/program.h) ---- */

void program_reset(void)
{
	if (!tables_loaded)
	{
		const char *problem = load_tables();

		if (problem)
		{
			fprintf(stderr, "The game does not start: %s\n", problem);
			exit(1);
		}
		tables_loaded = true;
	}
	status = 0x2700u;
	depth = 0;
	interrupts = 0;
	tmnt_board.program_outside = code_read;
	if (getenv("TMNT_WATCH"))
	{
		watched = (uint32_t)strtoul(getenv("TMNT_WATCH"), NULL, 16);
		watched_from = getenv("TMNT_WATCH_FRAME") ? strtoull(getenv("TMNT_WATCH_FRAME"), NULL, 10) : 0;
		tmnt_board.write_watch = watch_write;
	}
}

static void run_program(void)
{
	game_entry();
	fprintf(stderr, "68000 stopped: the program came to its end (frame %llu)\n", (unsigned long long)tmnt_board.frame);
	exit(3);
}

void program_run(void)
{
	port_task = host_task_main();
	host_task_switch(host_task_create(run_program));
}

void program_stop(void)
{
	host_task_switch(port_task);
}

uint64_t program_frames(void)
{
	return tmnt_board.frame;
}

uint32_t program_interrupts(void)
{
	return interrupts;
}
