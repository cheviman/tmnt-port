/**
 *    Name:        controls.c
 *    Description: the keys and game controller buttons of each player's controls, and the file that keeps them
 *                 (controls.ini)
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    As outrunners-port's (its ports/sdl2/controls.c), for the cabinet's four players: the defaults below;
 *    controls.ini, in the folder of what the game keeps, keeps what TMNT's CONTROLS page changes: a section a player,
 *    "[player 1]", and a line a control, "attack = Z, Left Ctrl | a" (SDL's names of the keys, then of the buttons and
 *    the triggers). A player's game controller is the one plugged in in their turn; its left stick moves besides
 *    (the window port).
 */
#include "controls.h"

#include <stdio.h>
#include <string.h>

#define NO_KEY     SDL_SCANCODE_UNKNOWN
#define NO_BUTTON  SDL_CONTROLLER_BUTTON_INVALID
#define LINE_SIZE  256
#define FILE_VERSION  1

/* The controls' names in controls.ini */
static const char *const control_names[CONTROLS] = {
	[CONTROL_UP] = "up", [CONTROL_DOWN] = "down", [CONTROL_LEFT] = "left", [CONTROL_RIGHT] = "right",
	[CONTROL_ATTACK] = "attack", [CONTROL_JUMP] = "jump", [CONTROL_START] = "start",
};

/* MAME's keys for the four players: player 1 the arrows, Z or Left Ctrl attack, X or Left Alt jump, Enter or 1;
   player 2 R F D G, A, S, 2; player 3 I K J L, Right Ctrl, Right Shift, 3; player 4 the keypad's 8 2 4 6, 0 and its
   point, 4 */
static const SDL_Scancode default_keys[CONTROL_PLAYERS][CONTROLS][CONTROL_KEYS] = {
	{
		[CONTROL_UP] = { SDL_SCANCODE_UP }, [CONTROL_DOWN] = { SDL_SCANCODE_DOWN },
		[CONTROL_LEFT] = { SDL_SCANCODE_LEFT }, [CONTROL_RIGHT] = { SDL_SCANCODE_RIGHT },
		[CONTROL_ATTACK] = { SDL_SCANCODE_Z, SDL_SCANCODE_LCTRL }, [CONTROL_JUMP] = { SDL_SCANCODE_X, SDL_SCANCODE_LALT },
		[CONTROL_START] = { SDL_SCANCODE_RETURN, SDL_SCANCODE_1 },
	},
	{
		[CONTROL_UP] = { SDL_SCANCODE_R }, [CONTROL_DOWN] = { SDL_SCANCODE_F },
		[CONTROL_LEFT] = { SDL_SCANCODE_D }, [CONTROL_RIGHT] = { SDL_SCANCODE_G },
		[CONTROL_ATTACK] = { SDL_SCANCODE_A }, [CONTROL_JUMP] = { SDL_SCANCODE_S }, [CONTROL_START] = { SDL_SCANCODE_2 },
	},
	{
		[CONTROL_UP] = { SDL_SCANCODE_I }, [CONTROL_DOWN] = { SDL_SCANCODE_K },
		[CONTROL_LEFT] = { SDL_SCANCODE_J }, [CONTROL_RIGHT] = { SDL_SCANCODE_L },
		[CONTROL_ATTACK] = { SDL_SCANCODE_RCTRL }, [CONTROL_JUMP] = { SDL_SCANCODE_RSHIFT },
		[CONTROL_START] = { SDL_SCANCODE_3 },
	},
	{
		[CONTROL_UP] = { SDL_SCANCODE_KP_8 }, [CONTROL_DOWN] = { SDL_SCANCODE_KP_2 },
		[CONTROL_LEFT] = { SDL_SCANCODE_KP_4 }, [CONTROL_RIGHT] = { SDL_SCANCODE_KP_6 },
		[CONTROL_ATTACK] = { SDL_SCANCODE_KP_0 }, [CONTROL_JUMP] = { SDL_SCANCODE_KP_PERIOD },
		[CONTROL_START] = { SDL_SCANCODE_4 },
	},
};

/* (a control with one button: none is SDL_CONTROLLER_BUTTON_INVALID, not 0, which is A) */
#define ONE_BUTTON(first)  { first, NO_BUTTON, NO_BUTTON }

/* Every player's game controller: the pad moves (as the stick does), A attacks, B jumps, Start */
static const SDL_GameControllerButton default_buttons[CONTROLS][CONTROL_BUTTONS] = {
	[CONTROL_UP] = ONE_BUTTON(SDL_CONTROLLER_BUTTON_DPAD_UP), [CONTROL_DOWN] = ONE_BUTTON(SDL_CONTROLLER_BUTTON_DPAD_DOWN),
	[CONTROL_LEFT] = ONE_BUTTON(SDL_CONTROLLER_BUTTON_DPAD_LEFT),
	[CONTROL_RIGHT] = ONE_BUTTON(SDL_CONTROLLER_BUTTON_DPAD_RIGHT),
	[CONTROL_ATTACK] = ONE_BUTTON(SDL_CONTROLLER_BUTTON_A), [CONTROL_JUMP] = ONE_BUTTON(SDL_CONTROLLER_BUTTON_B),
	[CONTROL_START] = ONE_BUTTON(SDL_CONTROLLER_BUTTON_START),
};

control_binding control_bindings[CONTROL_PLAYERS][CONTROLS];

static char file_path[1024];

static void set_defaults(int player)
{
	for (int control = 0; control < CONTROLS; control++)
	{
		control_binding *binding = &control_bindings[player][control];

		binding->name = control_names[control];
		for (int key = 0; key < CONTROL_KEYS; key++)
			binding->keys[key] = default_keys[player][control][key];    /* (0 is SDL_SCANCODE_UNKNOWN: none) */
		for (int button = 0; button < CONTROL_BUTTONS; button++)
			binding->buttons[button] = default_buttons[control][button];
	}
}

static bool is_trigger(SDL_GameControllerButton button)
{
	return button == CONTROL_LEFT_TRIGGER || button == CONTROL_RIGHT_TRIGGER;
}

bool controls_holds(int player, SDL_GameController *pad, int control)
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	const control_binding *binding = &control_bindings[player][control];

	for (int key = 0; key < CONTROL_KEYS; key++)
		if (binding->keys[key] != NO_KEY && keys[binding->keys[key]])
			return true;
	for (int button = 0; pad && button < CONTROL_BUTTONS; button++)
	{
		const SDL_GameControllerButton which = binding->buttons[button];

		if (which == NO_BUTTON)
			continue;
		if (is_trigger(which) ? SDL_GameControllerGetAxis(pad, which == CONTROL_LEFT_TRIGGER
				? SDL_CONTROLLER_AXIS_TRIGGERLEFT : SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > TRIGGER_PRESSED
			: SDL_GameControllerGetButton(pad, which) != 0)
			return true;
	}
	return false;
}

/* ---- controls.ini ---- */

static char *trimmed(char *text)
{
	char *end;

	while (*text == ' ' || *text == '\t')
		text++;
	end = text + strlen(text);
	while (end > text && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\r' || end[-1] == '\n'))
		*--end = 0;
	return text;
}

/* A button's name in controls.ini: SDL's, the triggers' its axes' ("lefttrigger", "righttrigger") */
static const char *button_name(SDL_GameControllerButton button)
{
	if (is_trigger(button))
		return SDL_GameControllerGetStringForAxis(button == CONTROL_LEFT_TRIGGER
			? SDL_CONTROLLER_AXIS_TRIGGERLEFT : SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	return SDL_GameControllerGetStringForButton(button);
}

static SDL_GameControllerButton named_button(const char *name)
{
	const SDL_GameControllerAxis axis = SDL_GameControllerGetAxisFromString(name);

	if (axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
		return CONTROL_LEFT_TRIGGER;
	if (axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
		return CONTROL_RIGHT_TRIGGER;
	return SDL_GameControllerGetButtonFromString(name);
}

/* One line "name = Key, Key | button, button": the player's control it names takes those, in place of its own */
static void read_line(int player, char *line)
{
	char *equals = strchr(line, '='), *bar, *part, *next;
	const char *name;
	control_binding *binding;
	int control, slot;

	if (line[0] == ';' || line[0] == '#' || !equals)
		return;
	*equals = 0;
	name = trimmed(line);
	for (control = 0; control < CONTROLS && strcmp(control_names[control], name) != 0; control++)
		;
	if (control == CONTROLS)
		return;
	binding = &control_bindings[player][control];
	bar = strchr(equals + 1, '|');
	if (bar)
		*bar = 0;
	for (slot = 0; slot < CONTROL_KEYS; slot++)
		binding->keys[slot] = NO_KEY;
	for (slot = 0; slot < CONTROL_BUTTONS; slot++)
		binding->buttons[slot] = NO_BUTTON;
	for (part = equals + 1, slot = 0; part && slot < CONTROL_KEYS; part = next)
	{
		next = strchr(part, ',');
		if (next)
			*next++ = 0;
		if (*trimmed(part))
			binding->keys[slot++] = SDL_GetScancodeFromName(trimmed(part));
	}
	for (part = bar ? bar + 1 : NULL, slot = 0; part && slot < CONTROL_BUTTONS; part = next)
	{
		next = strchr(part, ',');
		if (next)
			*next++ = 0;
		if (*trimmed(part))
			binding->buttons[slot++] = named_button(trimmed(part));
	}
}

void controls_open(const char *path)
{
	FILE *file;
	char line[LINE_SIZE];
	int player = 0, number = 0;

	for (int each = 0; each < CONTROL_PLAYERS; each++)
		set_defaults(each);
	SDL_strlcpy(file_path, path, sizeof(file_path));
	file = fopen(file_path, "r");
	if (!file)
	{
		controls_save();
		return;
	}
	while (fgets(line, sizeof(line), file))
	{
		/* "[player N]" starts that player's lines (the lines before any are player 1's) */
		if (sscanf(trimmed(line), "version = %d", &number) == 1)
			continue;
		if (sscanf(trimmed(line), "[player %d]", &number) == 1)
			player = number >= 1 && number <= CONTROL_PLAYERS ? number - 1 : -1;
		else if (player >= 0)
			read_line(player, line);
	}
	fclose(file);
}

void controls_save(void)
{
	FILE *file = fopen(file_path, "w");

	if (!file)
		return;
	fputs("; TMNT's controls, a section a player: a control a line, its keys (SDL's names), then after | its game\n"
		"; controller buttons and triggers. OPTIONS > CONTROLS writes this file; it can be changed by hand too.\n", file);
	fprintf(file, "version = %d\n", FILE_VERSION);
	for (int player = 0; player < CONTROL_PLAYERS; player++)
	{
		fprintf(file, "\n[player %d]\n", player + 1);
		for (int control = 0; control < CONTROLS; control++)
		{
			const control_binding *binding = &control_bindings[player][control];
			bool first = true;

			fprintf(file, "%s =", binding->name);
			for (int key = 0; key < CONTROL_KEYS; key++)
				if (binding->keys[key] != NO_KEY)
				{
					fprintf(file, "%s %s", first ? "" : ",", SDL_GetScancodeName(binding->keys[key]));
					first = false;
				}
			fputs(" |", file);
			first = true;
			for (int button = 0; button < CONTROL_BUTTONS; button++)
				if (binding->buttons[button] != NO_BUTTON)
				{
					fprintf(file, "%s %s", first ? "" : ",", button_name(binding->buttons[button]));
					first = false;
				}
			fputs("\n", file);
		}
	}
	fclose(file);
}

void controls_defaults(int player)
{
	set_defaults(player);
	controls_save();
}

/* The key or button the player pressed for 'control': its first one, the ones it had before moving on (one that was
   already the control's moves to the front, the last falls off); the player's other controls let go of it */
void controls_bind_key(int player, int control, SDL_Scancode key)
{
	control_binding *binding = &control_bindings[player][control];
	int slot;

	for (int other = 0; other < CONTROLS; other++)
	{
		SDL_Scancode *keys = control_bindings[player][other].keys;

		for (slot = CONTROL_KEYS - 1; other != control && slot >= 0; slot--)
			if (keys[slot] == key)
			{
				memmove(&keys[slot], &keys[slot + 1], (size_t)(CONTROL_KEYS - 1 - slot) * sizeof(keys[0]));
				keys[CONTROL_KEYS - 1] = NO_KEY;
			}
	}
	for (slot = 0; slot < CONTROL_KEYS - 1 && binding->keys[slot] != key; slot++)
		;
	memmove(&binding->keys[1], &binding->keys[0], (size_t)slot * sizeof(binding->keys[0]));
	binding->keys[0] = key;
	controls_save();
}

void controls_bind_button(int player, int control, SDL_GameControllerButton button)
{
	control_binding *binding = &control_bindings[player][control];
	int slot;

	for (int other = 0; other < CONTROLS; other++)
	{
		SDL_GameControllerButton *buttons = control_bindings[player][other].buttons;

		for (slot = CONTROL_BUTTONS - 1; other != control && slot >= 0; slot--)
			if (buttons[slot] == button)
			{
				memmove(&buttons[slot], &buttons[slot + 1], (size_t)(CONTROL_BUTTONS - 1 - slot) * sizeof(buttons[0]));
				buttons[CONTROL_BUTTONS - 1] = NO_BUTTON;
			}
	}
	for (slot = 0; slot < CONTROL_BUTTONS - 1 && binding->buttons[slot] != button; slot++)
		;
	memmove(&binding->buttons[1], &binding->buttons[0], (size_t)slot * sizeof(binding->buttons[0]));
	binding->buttons[0] = button;
	controls_save();
}
