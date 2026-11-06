/**
 *    Name:        lan.c
 *    Description: the LAN game (guideline 20): the menu's LAN, its rooms through arcade-lan, and its game played in
 *                 lockstep, every device the same frames from the same start
 *    Version:     0.1.0
 *    Created:     06-11-2026
 *    Modified:    06-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    A room's devices, up to four, each with its player: HOST makes one, JOIN finds them on the network, the host's
 *    START begins the game (console/PLAN.md, the LAN game). Every device then starts from the same state: the board
 *    reset (the port's program_run again from the program's start), the console as at power-on, the host's settings
 *    that change the play (the DIP switches, the region, WIDE), and Splinter Way's turtle choice open, its first
 *    player the host's. Each frame on, every device gives its player's controls (his stick and buttons, and START
 *    newly pressed) and runs the frame with every slot's: the device in a room's list order is the console's player in
 *    that order, on the turtle he picks. The state's hash after each frame goes to the host, which compares them. The
 *    game over (the attract mode again), the host gone or a device out of step, and the LAN page again. Without the
 *    library (TMNT_LAN not built) the page says NO NETWORK.
 */
#include "console_internal.h"

#ifdef TMNT_LAN
#include "arcade_lan.h"    /* (before game.h: ram.h's names are macros, its 'version' one) */
#endif
#include "game.h"
#include "tmnt/machine.h"

#include <stdio.h>
#include <string.h>

#ifdef TMNT_LAN

#define LAN_GAME_VERSION    1          /* the game's datagrams and play: another version's devices are not heard */
#define LAN_BEACON_PORT     27740
#define LAN_DELAY           3          /* frames: a player's controls play 3 frames after they are given */
#define LAN_BYTES           2          /* a player's input: his stick and buttons (TMNT_* bits), and START */
#define SETTINGS_BYTES      6          /* the host's: the DIP switches, the region, WIDE */

static struct
{
	lan_device *device;
	const char *problem;       /* what went wrong last, shown on the LAN page until a line is moved to */
	bool reset_wanted;         /* the game started: the board to be reset for it */
	bool playing;              /* its game, from the reset */
	bool game_begun;           /* the game itself (past the turtle choice) */
	bool frame_given;          /* a frame's inputs on the board: its state's hash after it */
	uint32_t frame;
	uint8_t settings[LAN_SETTINGS_MOST];
	size_t settings_size;
	int first_slot;
	uint8_t own_dsw[3];        /* this device's settings, back after the game */
	int own_region, own_wide;
	char waiting[LAN_NAME_LENGTH + 1];
} lan;

/* The device, opened when first wanted (its name the settings' NAME) */
static lan_device *device_of(void)
{
	static const lan_config config = { { 'T', 'M', 'N', 'T' }, LAN_GAME_VERSION, LAN_BEACON_PORT, LAN_BYTES,
		LAN_DELAY, 0 };

	if (!lan.device)
	{
		lan.device = lan_open(&config, console_settings.name[0] ? console_settings.name : CONSOLE_DEFAULT_NAME, 1);
		if (!lan.device)
			lan.problem = "NO NETWORK";
	}
	return lan.device;
}

static void take_problem(void)
{
	const char *problem = lan.device ? lan_problem(lan.device) : NULL;

	if (problem)
		lan.problem = problem;
}

bool console_lan_host(void)
{
	lan.problem = NULL;
	return device_of() && lan_host(lan.device);
}

void console_lan_search(void)
{
	lan.problem = NULL;
	if (device_of())
		lan_search(lan.device);
	take_problem();
}

void console_lan_leave(void)
{
	if (lan.device)
		lan_leave(lan.device);
}

/* NAME changed: the device opened again with it, when in no room */
void console_lan_name_changed(void)
{
	if (lan.device && lan_role_of(lan.device) == LAN_OFF)
	{
		lan_close(lan.device);
		lan.device = NULL;
	}
}

bool console_lan_searching(void)
{
	return lan_role_of(lan.device) == LAN_SEARCHING;
}

bool console_lan_in_room(void)
{
	return lan_role_of(lan.device) == LAN_IN_ROOM || lan_role_of(lan.device) == LAN_JOINING;
}

bool console_lan_joining(void)
{
	return lan_role_of(lan.device) == LAN_JOINING;
}

bool console_lan_hosting(void)
{
	return lan_hosting(lan.device);
}

int console_lan_room_count(void)
{
	lan_room rooms[LAN_ROOMS];

	return lan.device ? lan_rooms(lan.device, rooms) : 0;
}

/* A room found, as JOIN lists it: its host's name, its players, PLAYING when its game goes on */
void console_lan_room_text(int index, char *text, size_t size)
{
	lan_room rooms[LAN_ROOMS];
	const int count = lan.device ? lan_rooms(lan.device, rooms) : 0;

	if (index < 0 || index >= count)
	{
		snprintf(text, size, "?");
		return;
	}
	snprintf(text, size, "%-10s %d OF %d%s", rooms[index].name, rooms[index].players, LAN_PLAYERS,
		rooms[index].playing ? " PLAYING" : "");
}

bool console_lan_join(int index)
{
	lan_room rooms[LAN_ROOMS];
	const int count = lan.device ? lan_rooms(lan.device, rooms) : 0;

	if (index < 0 || index >= count || rooms[index].playing || rooms[index].players >= LAN_PLAYERS)
		return false;
	lan.problem = NULL;
	return lan_join(lan.device, &rooms[index]);
}

int console_lan_member_count(void)
{
	lan_member members[LAN_DEVICES];

	return lan.device ? lan_members(lan.device, members) : 0;
}

/* A device of the room, as the room's page lists it: its player's number, its name, HOST, YOU, WAITING when unheard */
void console_lan_member_text(int index, char *text, size_t size)
{
	lan_member members[LAN_DEVICES];
	const int count = lan.device ? lan_members(lan.device, members) : 0;

	if (index < 0 || index >= count)
	{
		snprintf(text, size, "?");
		return;
	}
	snprintf(text, size, "%dP %-10s%s%s%s", members[index].first_slot + 1, members[index].name,
		index == 0 ? " HOST" : "", members[index].own ? " YOU" : "", members[index].heard ? "" : " WAITING");
}

bool console_lan_can_start(void)
{
	return lan_hosting(lan.device) && lan_role_of(lan.device) == LAN_IN_ROOM && console_lan_member_count() > 1;
}

/* The host's START: its settings that change the play, sent to every device */
void console_lan_start(void)
{
	uint8_t settings[SETTINGS_BYTES];

	settings[0] = console_settings.dsw[0];
	settings[1] = console_settings.dsw[1];
	settings[2] = console_settings.dsw[2];
	settings[3] = (uint8_t)console_settings.region;
	settings[4] = (uint8_t)console_settings.wide;
	settings[5] = 0;    /* (the mode: Splinter Way's NORMAL) */
	if (console_lan_can_start())
		lan_start(lan.device, settings, sizeof(settings));
}

const char *console_lan_problem(void)
{
	take_problem();
	return lan.problem;
}

void console_lan_problem_seen(void)
{
	lan.problem = NULL;
}

/* Each frame on the LAN pages: the datagrams, and the game's start */
void console_lan_update(void)
{
	if (!lan.device)
		return;
	lan_update(lan.device);
	take_problem();
	if (lan_game_started(lan.device, lan.settings, &lan.settings_size, &lan.first_slot))
		lan.reset_wanted = true;
}

/* ---- the game ---- */

bool console_lan_reset_wanted(void)
{
	return lan.reset_wanted;
}

bool console_lan_playing(void)
{
	return lan.playing;
}

void console_lan_begin(void)
{
	lan.reset_wanted = false;
	lan.playing = true;
	lan.game_begun = false;
	lan.frame_given = false;
	memcpy(lan.own_dsw, console_settings.dsw, sizeof(lan.own_dsw));
	lan.own_region = console_settings.region;
	lan.own_wide = console_settings.wide;
	if (lan.settings_size >= SETTINGS_BYTES)
	{
		console_settings.dsw[0] = lan.settings[0];
		console_settings.dsw[1] = lan.settings[1];
		console_settings.dsw[2] = lan.settings[2];
		console_settings.region = lan.settings[3] < CONSOLE_REGIONS ? lan.settings[3] : 0;
		console_settings.wide = lan.settings[4] < WIDE_KINDS ? lan.settings[4] : WIDE_OFF;
	}
	console_power_on(true);
}

/* The game ended for this device: its own settings back, the LAN page */
static void end_game(const char *problem)
{
	if (lan.device)
		lan_leave(lan.device);
	lan.playing = false;
	lan.frame_given = false;
	memcpy(console_settings.dsw, lan.own_dsw, sizeof(lan.own_dsw));
	console_settings.region = lan.own_region;
	console_settings.wide = lan.own_wide;
	tmnt_board.dsw[0] = console_settings.dsw[0];
	tmnt_board.dsw[1] = console_settings.dsw[1];
	tmnt_board.dsw[2] = console_settings.dsw[2];
	if (problem)
		lan.problem = problem;
	console_back_to_lan_page();
}

void console_lan_game_over(void)
{
	end_game(NULL);
}

void console_lan_quit(void)
{
	console_quit_game();
	end_game(NULL);
}

bool console_lan_frame(uint8_t pressed, bool start)
{
	uint8_t own[LAN_BYTES], all[LAN_PLAYERS * LAN_BYTES], players[CONSOLE_PLAYERS], starts = 0;
	const char *problem;

	if (!lan.playing)
		return true;
	if (lan.frame_given)
	{
		lan_frame_hash(lan.device, lan.frame, tmnt_state_hash());
		lan.frame_given = false;
	}
	lan_update(lan.device);
	problem = lan_problem(lan.device);
	if (lan_role_of(lan.device) != LAN_PLAYING || (problem && strcmp(problem, "OUT OF STEP") == 0))
	{
		/* (the host gone, or a device out of step: the game ended where it is, for this device) */
		console_quit_game();
		end_game(problem ? problem : "HOST GONE");
		return true;
	}
	if (console.page == PAGE_GAME)
		lan.game_begun = true;
	own[0] = pressed;
	own[1] = start ? 1u : 0u;
	lan_give_inputs(lan.device, own);
	if (!lan_next_frame(lan.device, all))
	{
		if (!lan_waiting(lan.device, lan.waiting))
			lan.waiting[0] = '\0';
		return false;
	}
	lan.waiting[0] = '\0';
	lan.frame = lan_frame_number(lan.device) - 1u;
	lan.frame_given = true;
	for (int slot = 0; slot < CONSOLE_PLAYERS; slot++)
	{
		players[slot] = all[slot * LAN_BYTES];
		if (all[slot * LAN_BYTES + 1])
			starts |= (uint8_t)(1u << slot);
	}
	console_controls(players, starts);
	return true;
}

uint32_t console_lan_frame_number(void)
{
	return lan.frame;
}

const char *console_lan_waiting(void)
{
	return lan.playing && lan.waiting[0] ? lan.waiting : NULL;
}

#else

/* (no LAN in this build: the page says so) */
bool console_lan_host(void) { return false; }
void console_lan_search(void) { }
void console_lan_leave(void) { }
void console_lan_name_changed(void) { }
bool console_lan_searching(void) { return false; }
bool console_lan_in_room(void) { return false; }
bool console_lan_joining(void) { return false; }
bool console_lan_hosting(void) { return false; }
int console_lan_room_count(void) { return 0; }
void console_lan_room_text(int index, char *text, size_t size) { (void)index; snprintf(text, size, "?"); }
bool console_lan_join(int index) { (void)index; return false; }
int console_lan_member_count(void) { return 0; }
void console_lan_member_text(int index, char *text, size_t size) { (void)index; snprintf(text, size, "?"); }
bool console_lan_can_start(void) { return false; }
void console_lan_start(void) { }
const char *console_lan_problem(void) { return "NO NETWORK"; }
void console_lan_problem_seen(void) { }
void console_lan_update(void) { }
bool console_lan_reset_wanted(void) { return false; }
bool console_lan_playing(void) { return false; }
void console_lan_begin(void) { }
void console_lan_game_over(void) { }
void console_lan_quit(void) { }
bool console_lan_frame(uint8_t pressed, bool start) { (void)pressed; (void)start; return true; }
uint32_t console_lan_frame_number(void) { return 0; }
const char *console_lan_waiting(void) { return NULL; }

#endif
