"""
Name:        game_profile.py
Description: what the shared decompiler core (tools/core) needs to know of TMNT: where the game is, its ROM set,
             and its settings
Version:     0.1.0
Created:     29-10-2026
Modified:    01-11-2026
Author:      Sebastian Toyos

The core (arcade-decomp: the decompiler, the CPU frontends, the analysis, the checks) is the same for every
game; this module gives it TMNT's: the repository's root, the ROM set's module (tmnt_rom: the regions as the
board has them) and the names the generated files carry. The decompiler's settings for the 68000 program
(its structures, the registers the whole program shares, its variables) come here as the steps that need
them are reached; until then the core's neutral defaults stand.
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent / "core"))

import tmnt_rom as rom  # noqa: E402,F401

ROOT = Path(__file__).resolve().parents[1]
GAME = "Teenage Mutant Ninja Turtles (arcade, Konami 1989)"    # in the generated files' headers
ROM_SET_TITLE = "Teenage Mutant Ninja Turtles (tmnt, World 4 Players, version X)"
CPU = "68000"
BINARY = "tmnt"                                 # its executables: tmnt_game_headless, tmnt_headless
GUARD = "TMNT"                                  # the generated headers' include guards: TMNT_GAME_H...
REFERENCE = "kref"                              # the reference (tools/core/konami/reference)
ANALYSIS = "m68000.analysis"                    # the static analysis and the disassembler of the 68000
DISASSEMBLER = "m68000.disassembler"
CODE_START = 0x1000                             # 000400-000FFF, after the vectors, is all zeros
DATA_WALK = "data_walk"                         # the data the engines read, walked (tools/data_walk.py)
OBJECT_REGISTER = "A0"                          # the object a handler runs on: its fields, for the analysis
# a table of states stops at a place the code names as data even where that place holds a zero (the arcs right after
# four enemies' tables of states, 049BD8...: the analysis took their words for two more states; core/m68000/analysis.py)
TABLE_ENDS_BEFORE_UNUSED = True
# a family of objects whose name starts with another's (enemy_walk, enemy) reads the fields that one names too,
# besides its own (symbols.txt scoped lines; decompiler/pointers.py)
NESTED_FAMILIES = True
TRANSLATOR = "m68000.translate"                 # the translation scaffold (tools/recompile.py) for the 68000
FRAME_WAIT = 0x0013CE                           # the main loop finds no job here: the frame (runtime/include/tmnt/frame.h)
# The game's code the board puts its own in place of (runtime/src/board_code.c): where it starts, the board's
# function, and where the game's code goes on after it. The boot's ROM test sums the program's chips, which
# the game does not have: the board gives what good chips sum to.
REPLACED_CODE = {0x001850: ("sum_rom_chips", 0x00188E)}

# The decompiler (tools/decompile.py: the core's m68000/decompile.py) and the board as the program's C sees it
DECOMPILER = "m68000.decompile"
DECOMPILED = "29-10-2026"                       # when src/ was first written (its files' headers)
WORK_RAM_AREA = (0x060000, 0x064000)
PROGRAM_AREA = (0x000000, 0x060000)
# the board's devices, reached through the bus (their registers by the area's name and address)
HARDWARE = [(0x080000, 0x081000, "PALETTE"), (0x0A0000, 0x0A0020, "IO"), (0x0C0000, 0x0C0002, "PRIORITY"),
            (0x100000, 0x108000, "TILES"), (0x10E800, 0x10E802, "UNMAPPED"), (0x140000, 0x140008, "SPRITE_CONTROL"),
            (0x140400, 0x140800, "SPRITES")]
# a pointer the code works out into one of the board's devices (the K052109's tile RAM a text or a block of tiles
# is drawn into, the sprite RAM, the palette RAM) is the device's, reached through the bus: io_write8(p, value)
DEVICE_AREAS = [(start, end) for start, end, _ in HARDWARE]
# reading the K051937 and the K051960 does something (in the sprite ROM's readback mode, the read moves its
# offset): a read there stays where its value goes unused
EFFECT_READ_AREAS = [(0x140000, 0x140800)]
# the game's structures that lie one after another at a fixed address (symbols.txt declares them): the 50 objects, the
# players' records, the coin slots, the palette effects' slots, the palette requests, the players' slots the enemies use
STRUCT_ARRAYS = {"object": (0x062000, 50), "player": (0x060100, 4), "coin_slot": (0x060300, 4),
                 "palette_effect": (0x060480, 64), "palette_request": (0x060B00, 40), "player_slot": (0x062FA0, 4),
                 "layer": (0x063300, 3), "typed_pass": (0x063430, 8)}
# the objects at a fixed address that are always of one family: objects 0-3 are the players' turtles (turtle_join
# starts them, 019B3A "lea $62000.l, A0" to 019B88 "cmpa.l #$62140, A0"), whose fields an access by their address reads
INSTANCE_FAMILIES = {0x062000 + 0x50 * number: "turtle" for number in range(4)}
# and the ones of a family in some functions only: object 12 is the profile's picture in the attract's profiles
# (profile_start_picture: 00501A "lea $623c0.l, A0"), object 40 the van in the car street's exit
# (turtle_exit_car_street_board: 02C10E "lea $62c80.l, A6")
INSTANCE_FAMILIES_BY_FUNCTION = {(0x0623C0, "profile"): "profile", (0x062C80, "turtle_exit_car_street"): "car_street_van"}
# the structures at a fixed address the code reaches by their own names: the layer the code works on, a copy of
# layer A's or B's (the K052109's two scrolling layers; load_layer_a copies one in, save_layer_a back)
NAMED_BASES = {0x063300: "WORKING_LAYER", 0x063340: "LAYER_A", 0x063380: "LAYER_B"}
# the board's memories, as game/support.h names them (addresses the code computes into them)
REGIONS = [(0x080000, 0x081000, "PALETTE_RAM"), (0x100000, 0x108000, "TILE_RAM"), (0x140400, 0x140800, "SPRITE_RAM")]
# the VBLANK handler (003DC4) moves its return frame from the supervisor's stack to the user's (move USP, A6 ...
# move A6, USP) and goes on in user mode: the C has no stack, so that stretch is left out
INTERRUPT_STACK_SWITCH = (0x003DE6, 0x003DF4)
# the code that runs in supervisor mode only: the reset (up to 001380's move to SR, which leaves it), the VBLANK
# handler's start (up to its move to SR); a privileged instruction there needs no check of the mode
SUPERVISOR_CODE = ((0x001000, 0x001384), (0x003DC4, 0x003DF8))
# the routines that work on memory by its address, whatever is there (the faithful measure of tools/noise_report.py
# takes their memory reached by address as the original's), each with why
MEMORY_ROUTINES = {
    "zero_ram_below": "zeroes D0 + 1 blocks of 256 bytes below the address in A0 with movem (004516-004532), whatever is "
                      "there: its callers hand it the work RAM's stretches and the sprite RAM (004612 \"lea $140800.l, A0\")",
    "clear_objects": "zeroes the 50 objects with movem, 32 bytes at a time down from their end (004604 \"lea $62fa0.l, A0\", 00460A \"move.w #$7c, D0\")",
    # (five ways into one unreached tail, 004666: a place, a count, then three movem a block of 96 bytes, 00466A-004672)
    "unreached_004620": "unreached: zeroes 4 blocks of 96 bytes below 062000 (004620), whatever is there",
    "unreached_00462E": "unreached: zeroes 12 blocks of 96 bytes below 0623C0 (00462E), whatever is there",
    "unreached_00463C": "unreached: zeroes 8 blocks of 96 bytes below 062780 (00463C), whatever is there",
    "unreached_00464A": "unreached: zeroes 8 blocks of 96 bytes below 062140 (00464A), whatever is there",
    "unreached_004658": "unreached: zeroes 8 blocks of 96 bytes below 062A00 (004658), whatever is there",
}
# a constant stored into a field symbols.txt types as a number (an object's or a layer's x and y: positions, 1/256
# of a pixel a unit), or compared with what one holds, is a number, not the address of what the program has there
# (0x13C00 is where the street's exit is, not a table at 013C00)
NUMBERS_BY_FIELD = True
# a load through a register that holds a table's address on every way to it ("lea table, A6" before the loop that
# reads "(A6,D7.w)") reads that table by its name, wherever the register was set (decompiler/expressions.py)
CONSTANT_TABLE_BASES = True
# a table read with a signed index from a few bytes into a table or piece of data with a name ("(1,A1,D0.w)") reads as
# those bytes into it, by its name: TABLE8(fire_blocks, 0x1u + index) (core/m68000/emit.py)
NAMED_TABLE_OFFSETS = True
# tables of records (symbols.txt "ADDRESS table NAME STRUCT"): the loader turns each record to the host's order as a
# whole, as the structures run, and the C reads name[i].field (core/m68000/decompile.py's registry)
RECORD_TABLES = True
# the values a pointer walks one at a time (symbols.txt elements lines: the texts' bytes, the layers' blocks' tiles)
# read at their address by their name, TEXT_BYTE(p), as the structures' copies run (core/decompiler/pointers.py)
ELEMENT_READS = True
# a parameter or result symbols.txt does not name is called for what the function does with it, as a local is: the
# structure read through it (the object a handler runs on, a player's slot), or, for a value the function only passes
# on, the name the functions it hands it to agree on (core/decompiler/emit.py entry_hints)
PARAMS_BY_USE = True
# a variable or parameter nothing else names, whose values go into one field of a structure only, is called like that
# field (OBJECT(object)->move_time = move_time; core/decompiler/emit.py stored_names)
NAMES_BY_STORES = True
# a parameter nothing else names takes the name of what every caller passes in it, when they agree (a handler run from
# run_object_state's switch gets its slot)
PARAMS_FROM_CALLERS = True
# a variable nothing else names, compared with one field of a structure only, is called like it (if (OBJECT(object)->
# screen_x <= screen_x)); and the one a dbcc counts down is the loop's count (core/decompiler/emit.py)
NAMES_BY_COMPARISONS = True
LOOP_COUNTERS = True
# what a function reads to hand back a register is known part by part: enemy_face_object works out D7's low byte
# (the facing) and hands back its high word as it found it; a state handler that reads only the byte does not carry
# the word from the handler before it (core/decompiler/dataflow.py)
PASSES_BY_PART = True
# and a register written whole with a constant or'ed, and'ed or xor'ed writes only the parts the constant changes:
# "bset #7, D7" (enemy_facing_bits) writes D7's low byte, not the high word it leaves (core/decompiler/dataflow.py)
UNCHANGED_PARTS = True
# and what a function hands back is what its callers read once every function is known, not what they seemed to read
# while the outputs grew (queue_sound handed back A6, which no caller reads after it; core/decompiler/dataflow.py)
NARROW_OUTPUTS = True
NOISE_REGISTERS = r"[da][0-7]"                 # the 68000's: what tools/noise_report.py counts as named after a register
NOTES = {
    "variables": ["/* the game's variables in work RAM (060000-063FFF) */"],
    "scoped": ["/* work RAM as one family of functions uses it */"],
    "tables": ["/* the program's data the code reads by name, which the game loads from its assets */"],
    "hardware": ["/* the board's registers (read and written through the bus) */"],
}

# The Z80 sound program (963e20.g13, 32 KB at 0000-7FFF; the RAM at 8000): the reset and the interrupt (IM 1,
# the 68000's sound command) as on System 32, the program below 8000, and the jumps through registers followed
# by what the registers hold (the driver's channels keep the routine that runs them next in their record)
SOUND_ENTRIES = {0x0000: "func", 0x0038: "func"}
SOUND_CODE_END = 0x8000
SOUND_FOLLOW_VALUES = True
SOUND_DATA_WALK = "sound_data_walk"             # the sound data the driver reads, walked (tools/sound_data_walk.py)
# The decompiled sound program (tools/decompile_sound.py; it runs on runtime/src/sound_cpu.c): the Z80's own RAM
# holds its variables and its channel records, the program ROM its tables, which the game loads by name from its
# assets (src/sound/tables.c); the board's chips are registers on the Z80's bus (runtime/src/sound_board.c)
SOUND_RAM_AREA = (0x8000, 0x8800)
SOUND_ROM_AREA = (0x0000, 0x8000)
SOUND_HARDWARE = [(0x9000, 0x9001, "SOUND_CONTROL"), (0xA000, 0xA001, "SOUND_LATCH"), (0xB000, 0xB00E, "PCM"),
                  (0xC000, 0xC002, "FM"), (0xD000, 0xD001, "SPEECH"), (0xE000, 0xE001, "SPEECH_START"),
                  (0xF000, 0xF001, "SPEECH_BUSY")]
SOUND_NOTES = {
    "variables": ["/* the sound program's variables and channel records, in the Z80's RAM (8000-87FF) */"],
    "scoped": ["/* RAM as one family of functions uses it */"],
    "tables": ["/* data in the program ROM the code reads, which the game loads from its assets */"],
    "hardware": ["/* the board's registers on the Z80's bus: 9000 (sres), the 68000's latch, the K007232, the",
                 "   YM2151 and the uPD7759 (read and written through the bus) */"],
}
SOUND_LOADED_TABLES = True
SOUND_BLOCK_FETCHES = False                     # no LD A,R: the refresh register is never read
SOUND_INTERRUPT_WINDOWS = True                  # the main loop lets the 68000's command in for four NOPs only (EI ... DI)
SOUND_COUNTED_FROM_ONE = True                   # tables read from an element before them: note_lengths[i - 1]
SOUND_KEEP_HARDWARE_READS = True                # reading the K007232's key (B005, B00B) starts its channel: such a read stays
SOUND_NAMES_BEFORE_TABLES = 16                  # the base of a table indexed from past its start: pcm_note_pitches_ADDRESS - 0xCu
SOUND_TABLE_REGISTERS = True                    # a table read through HL ("ld hl,TABLE; add hl,de; ld e,(hl)"), by its name
SOUND_CREATED = "29-10-2026"                    # when src/sound/ was first written (its files' headers)
# The 15 channel records (sound_symbols.txt's array line) are structures at a fixed address too: an access by the
# address of a field of one reads that field, by the name of what plays on the record (sound_main gives each its chip
# channel, 047B-04C4 "ld hl,$8000; ld (hl),$00" to "ld hl,$8380; ld (hl),$07": 8000 the YM2151's 0, 8040-81C0 1-7,
# 8200 and 8240 the K007232's A and B (8, 9), 8280 and 82C0 A and B again (A, B), 8300 the speech (C), 8340 and 8380
# the YM2151's 6 and 7 again): the FM effects' first record (start_fm_effect 0281 "ld ix,$8000"), the music's 9
# (start_music 01D7 "ld ix,$8040", 01D5 "ld b,$09"), the PCM effects' 2 (start_pcm_effect 022C "ld ix,$8280", 0233
# "ld ix,$82C0"), the speech's (start_speech 00D7 "ld ix,$8300") and the FM effects' 2 on the music's channels 6 and 7
# (start_fm_effect 028F "ld ix,$8340", 02A1 "ld ix,$8380")
SOUND_INSTANCES = True
SOUND_NAMED_BASES = {0x8000: "FM_EFFECT_0", 0x8040: "MUSIC_FM_1", 0x8080: "MUSIC_FM_2", 0x80C0: "MUSIC_FM_3",
                     0x8100: "MUSIC_FM_4", 0x8140: "MUSIC_FM_5", 0x8180: "MUSIC_FM_6", 0x81C0: "MUSIC_FM_7",
                     0x8200: "MUSIC_PCM_A", 0x8240: "MUSIC_PCM_B", 0x8280: "PCM_EFFECT_A", 0x82C0: "PCM_EFFECT_B",
                     0x8300: "SPEECH", 0x8340: "FM_EFFECT_6", 0x8380: "FM_EFFECT_7"}
# every channel's handler runs on its record in IX (run_channel_handler 05C9-05CF "ld l,(ix+2); ld h,(ix+3); jp (hl)"):
# what the music, the FM effects and the PCM effects keep past +10 means one thing to each (sound_symbols.txt's
# scoped lines, by the families of the handlers' names)
SOUND_FAMILY_STRUCT = "channel"
SOUND_OBJECT_REGISTER = "ix"
SOUND_NESTED_FAMILIES = True
# the names of what the code does with a value (decompiler/emit.py, as the 68000's PARAMS_BY_USE and the rest): the
# record a handler runs on is its channel, a value stored into one field only is what that field holds, a loop's DJNZ
# counts its count
SOUND_PARAMS_BY_USE = True
SOUND_NAMES_BY_STORES = True
SOUND_PARAMS_FROM_CALLERS = True
SOUND_NAMES_BY_COMPARISONS = True
SOUND_LOOP_COUNTERS = True
# a value a function keeps in a variable as it is and hands to a routine is what that routine calls it (fm_effect_02_slide:
# "ld ($83E4),hl; jp set_fm_pitch", the pitch)
SOUND_NAMES_PAST_STORES = True
# an access through HL set to a variable's address on every way to it ("ld hl,$83DF; dec (hl)", then "ld a,(hl)" past
# the jump) reads that variable, as the C names it: HL is no longer set for it (decompiler/expressions.py)
SOUND_CONSTANT_ADDRESSES = True
