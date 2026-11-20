/**
 *    Name:        touch.c
 *    Description: the touch screen's controls (Android): a stick where the left thumb lands, ATTACK and JUMP at the
 *                 right, START (TMNT) or COIN (TMNT Arcade) over them, drawn over the picture, and what the fingers
 *                 on them hold: player 1's controls
 *    Version:     0.2.0
 *    Created:     06-11-2026
 *    Modified:    07-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    As outrunners-port's (its wheel and pedals for a car; here the cabinet's stick and two buttons). A finger that
 *    lands on the screen's left half is the stick: its centre where it landed, its way the finger's from there (eight,
 *    as the cabinet's); one on a button holds it, and sliding onto the other holds that one. On a phone the controls
 *    are drawn from the screen's sides, over the picture's corners; on a screen taller than the picture by a fifth or
 *    more (an open Fold's, a tablet's) the picture goes to the top and the controls into the room under it (as
 *    outrunners-port's). Their size is the area's smaller side's (its height, or its width over AREA_ASPECT), and
 *    never more than MOST_UNIT_INCHES a unit on the screen (Sebas: on a Fold they were huge). A game controller or a
 *    key hides them till a finger touches the screen again. The letters are font.bmp's (Kenney Mini, ports/sdl/font).
 */
#include "touch.h"

#include "letters.h"
#include "tmnt/machine.h"

#include <math.h>
#include <string.h>

#define FINGERS             10
#define STICK_DEAD          0.30       /* of the stick's reach: no way within it */
#define STICK_DIAGONAL      0.38       /* the share of the way's other axis that makes it a diagonal (cos 67.5) */
#define ROOM_UNDER          0.20f      /* the room under the picture, in the screen's height, that takes the controls */
#define AREA_ASPECT         1.8f       /* over the picture, the unit: a tenth of the height, or of the width over this */
#define UNDER_UNITS         4.0f       /* under the picture, the unit: the room's height over this */
#define MOST_UNIT_INCHES    0.35f      /* and at most this on the screen (a unit: the stick's reach is 1.3, a button's
                                          radius 1.05) */

#ifdef TMNT_CONSOLE
#define START_LABEL         "START"
#else
#define START_LABEL         "COIN"
#endif

enum { HOLD_NOTHING, HOLD_STICK, HOLD_ATTACK, HOLD_JUMP, HOLD_START };

/* The controls on the screen now (touch_place), in its pixels */
typedef struct
{
	float x, y, radius;
} circle;

static struct
{
	bool shown;
	letters font;
	bool have_font;
	int width, height;               /* (the screen they were placed on) */
	SDL_Rect area;                   /* the controls': the screen, or the room under the picture */
	bool under;                      /* (the area is the room under the picture) */
	float most_unit;                 /* MOST_UNIT_INCHES in the screen's pixels (none known: 0) */
	circle stick_home, attack, jump, start;
	float stick_reach;
	struct
	{
		SDL_FingerID id;
		int holds;
		float x0, y0, x, y;            /* (the stick's: where it landed, where it is) */
	} fingers[FINGERS];
} touch;

/* The screen's dots an inch: Android's density (SDL 3's content scale of the display) times its 160; 0 unknown.
   SDL 2's SDL_GetDisplayDPI is gone, and with it the screen's own measure: the density is the one Android lays its
   own controls out by */
static float screen_dots(void)
{
#ifdef SDL_PLATFORM_IOS
	/* (iOS: the screen's points, 163 an inch on an iPhone and 132 on an iPad, times the pixels a point is) */
	const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());

	return (SDL_IsTablet() ? 132.0f : 163.0f) * (mode && mode->pixel_density > 0.0f ? mode->pixel_density : 1.0f);
#else
	return 160.0f * SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
#endif
}

/* Where the controls go in their area (touch_layout): over the picture, from the screen's sides and corners; under
   it, along the middle of the room, START (COIN) between the stick and the buttons */
static void place(void)
{
	const SDL_Rect *area = &touch.area;
	const float width = (float)touch.width;
	float unit = touch.under ? (float)area->h / UNDER_UNITS
		: SDL_min((float)area->h, (float)area->w / AREA_ASPECT) / 10.0f;

	if (touch.most_unit > 0.0f)
		unit = SDL_min(unit, touch.most_unit);
	touch.stick_reach = unit * 1.3f;
	if (touch.under)
	{
		const float middle = (float)area->y + (float)area->h / 2.0f;

		touch.stick_home = (circle){ unit * 2.2f, middle, touch.stick_reach };
		touch.attack = (circle){ width - unit * 1.6f, middle - unit * 0.3f, unit * 1.05f };
		touch.jump = (circle){ width - unit * 3.9f, middle + unit * 0.3f, unit * 1.05f };
		touch.start = (circle){ width / 2.0f, middle, unit * 0.7f };
		return;
	}
	touch.stick_home = (circle){ unit * 2.2f, (float)touch.height - unit * 2.4f, touch.stick_reach };
	touch.attack = (circle){ width - unit * 1.6f, (float)touch.height - unit * 2.0f, unit * 1.05f };
	touch.jump = (circle){ width - unit * 3.9f, (float)touch.height - unit * 1.4f, unit * 1.05f };
	touch.start = (circle){ width - unit * 1.6f, unit * 1.3f, unit * 0.7f };
}

void touch_layout(int width, int height, SDL_Rect *picture)
{
	const bool under = touch.shown && height - picture->h >= (int)(height * ROOM_UNDER);
	const SDL_Rect area = under ? (SDL_Rect){ 0, picture->h, width, height - picture->h } : (SDL_Rect){ 0, 0, width,
		height };

	if (under)
		picture->y = 0;    /* (the picture at the top, the controls in the room under it) */
	if (width == touch.width && height == touch.height && under == touch.under && area.y == touch.area.y
		&& area.h == touch.area.h)
		return;
	if (width != touch.width || height != touch.height)
	{
		/* (another screen, a Fold opened or closed: its own density) */
		const float dots = screen_dots();

		if (dots > 0.0f)
			touch.most_unit = dots * MOST_UNIT_INCHES;
	}
	touch.width = width;
	touch.height = height;
	touch.under = under;
	touch.area = area;
	place();
}

static bool inside(const circle *button, float x, float y, float more)
{
	const float dx = x - button->x, dy = y - button->y, reach = button->radius * more;

	return dx * dx + dy * dy <= reach * reach;
}

/* What a finger at x, y on the screen holds as it lands there: a button under it, else the stick on the left half */
static int under(float x, float y)
{
	if (inside(&touch.start, x, y, 1.3f))
		return HOLD_START;
	if (inside(&touch.attack, x, y, 1.25f))
		return HOLD_ATTACK;
	if (inside(&touch.jump, x, y, 1.25f))
		return HOLD_JUMP;
	return x < (float)touch.width / 2.0f ? HOLD_STICK : HOLD_NOTHING;
}

void touch_open(SDL_Renderer *renderer, const char *base)
{
	const float dots = screen_dots();
	int count = 0;

	touch.have_font = letters_load(renderer, base, &touch.font) != 0;
	SDL_free(SDL_GetTouchDevices(&count));    /* (SDL's list of the touch screens: how many) */
	touch.shown = count > 0;
	if (dots > 0.0f)
		touch.most_unit = dots * MOST_UNIT_INCHES;
}

void touch_event(const SDL_Event *event)
{
	switch (event->type)
	{
		case SDL_EVENT_FINGER_DOWN:
		case SDL_EVENT_FINGER_MOTION:
		case SDL_EVENT_FINGER_UP:
		{
			const float x = event->tfinger.x * (float)touch.width, y = event->tfinger.y * (float)touch.height;
			int index = -1, free_one = -1;

			touch.shown = true;
			for (int finger = 0; finger < FINGERS; finger++)
			{
				if (touch.fingers[finger].holds != HOLD_NOTHING && touch.fingers[finger].id == event->tfinger.fingerID)
					index = finger;
				else if (touch.fingers[finger].holds == HOLD_NOTHING && free_one < 0)
					free_one = finger;
			}
			if (event->type == SDL_EVENT_FINGER_UP)
			{
				if (index >= 0)
					touch.fingers[index].holds = HOLD_NOTHING;
				break;
			}
			if (index < 0)
			{
				if (event->type != SDL_EVENT_FINGER_DOWN || free_one < 0 || (touch.fingers[free_one].holds = under(x, y))
					== HOLD_NOTHING)
					break;
				index = free_one;
				touch.fingers[index].id = event->tfinger.fingerID;
				touch.fingers[index].x0 = x;
				touch.fingers[index].y0 = y;
			}
			touch.fingers[index].x = x;
			touch.fingers[index].y = y;
			if (touch.fingers[index].holds == HOLD_ATTACK || touch.fingers[index].holds == HOLD_JUMP)
			{
				/* (a thumb sliding from one button onto the other) */
				if (inside(&touch.attack, x, y, 1.25f))
					touch.fingers[index].holds = HOLD_ATTACK;
				else if (inside(&touch.jump, x, y, 1.25f))
					touch.fingers[index].holds = HOLD_JUMP;
			}
			break;
		}
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		case SDL_EVENT_KEY_DOWN:
			touch.shown = false;
			memset(touch.fingers, 0, sizeof(touch.fingers));
			break;
		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
			if (event->gaxis.value > 16000 || event->gaxis.value < -16000)
				touch.shown = false;
			break;
		default:
			break;
	}
}

uint8_t touch_controls(void)
{
	uint8_t held = 0;

	for (int finger = 0; finger < FINGERS; finger++)
		switch (touch.fingers[finger].holds)
		{
			case HOLD_STICK:
			{
				const float dx = touch.fingers[finger].x - touch.fingers[finger].x0;
				const float dy = touch.fingers[finger].y - touch.fingers[finger].y0;
				const float length = sqrtf(dx * dx + dy * dy);

				if (length < touch.stick_reach * STICK_DEAD)
					break;
				if (dx < -length * STICK_DIAGONAL)
					held |= TMNT_LEFT;
				else if (dx > length * STICK_DIAGONAL)
					held |= TMNT_RIGHT;
				if (dy < -length * STICK_DIAGONAL)
					held |= TMNT_UP;
				else if (dy > length * STICK_DIAGONAL)
					held |= TMNT_DOWN;
				break;
			}
			case HOLD_ATTACK:
				held |= TMNT_ATTACK;
				break;
			case HOLD_JUMP:
				held |= TMNT_JUMP;
				break;
			default:
				break;
		}
	return held;
}

bool touch_start(void)
{
	for (int finger = 0; finger < FINGERS; finger++)
		if (touch.fingers[finger].holds == HOLD_START)
			return true;
	return false;
}

/* A disc of 'colour', its edge a little stronger */
static void disc(SDL_Renderer *renderer, const circle *where, SDL_Color colour)
{
	const int radius = (int)where->radius;

	for (int dy = -radius; dy <= radius; dy++)
	{
		const int dx = (int)sqrtf((float)(radius * radius - dy * dy));

		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderLine(renderer, (float)((int)where->x - dx), (float)((int)where->y + dy), (float)((int)where->x + dx),
			(float)((int)where->y + dy));
		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, (Uint8)SDL_min(255, colour.a * 2));
		SDL_RenderPoint(renderer, (float)((int)where->x - dx), (float)((int)where->y + dy));
		SDL_RenderPoint(renderer, (float)((int)where->x + dx), (float)((int)where->y + dy));
	}
}

/* The letters' scale on a button: as large as the button makes them, but 'widest' (the longest name of the buttons
   of its size) inside eight tenths of its width. Sebas: "el attack queda muy grande en el boton" */
static int label_scale(const circle *where, const char *widest)
{
	int scale = SDL_max(1, (int)(where->radius / 22.0f));

	while (scale > 1 && letters_width(&touch.font, widest, scale) > (int)(where->radius * 1.6f))
		scale--;
	return scale;
}

/* A button's name, centred on it, its letters 'scale' times the font's */
static void label(SDL_Renderer *renderer, const circle *where, const char *text, int scale)
{
	const int width = letters_width(&touch.font, text, scale);
	const SDL_Rect area = { (int)where->x - width / 2, (int)where->y - LETTERS_CELL * scale / 2, width,
		LETTERS_CELL * scale };
	const SDL_Color white = { 255, 255, 255, 220 };

	if (!touch.have_font)
		return;
	SDL_SetRenderViewport(renderer, &area);
	letters_draw(renderer, &touch.font, text, 0, scale, white, width);
	SDL_SetRenderViewport(renderer, NULL);
}

void touch_draw(SDL_Renderer *renderer, int width, int height)
{
	const SDL_Color idle = { 255, 255, 255, 50 }, held = { 255, 230, 120, 110 };
	const uint8_t holding = touch_controls();
	const bool start_held = touch_start();
	circle stick = { 0 }, knob;
	SDL_Rect whole = { 0, 0, width, height };

	if (!touch.width)
		touch_layout(width, height, &whole);    /* (none laid out yet: the whole screen's) */
	if (!touch.shown)
		return;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	stick = touch.stick_home;
	knob = (circle){ stick.x, stick.y, touch.stick_reach * 0.45f };
	for (int finger = 0; finger < FINGERS; finger++)
		if (touch.fingers[finger].holds == HOLD_STICK)
		{
			float dx = touch.fingers[finger].x - touch.fingers[finger].x0;
			float dy = touch.fingers[finger].y - touch.fingers[finger].y0;
			const float length = sqrtf(dx * dx + dy * dy);

			if (length > touch.stick_reach)
			{
				dx *= touch.stick_reach / length;
				dy *= touch.stick_reach / length;
			}
			stick.x = touch.fingers[finger].x0;
			stick.y = touch.fingers[finger].y0;
			knob = (circle){ stick.x + dx, stick.y + dy, touch.stick_reach * 0.45f };
		}
	disc(renderer, &stick, idle);
	disc(renderer, &knob, (holding & (TMNT_LEFT | TMNT_RIGHT | TMNT_UP | TMNT_DOWN)) ? held : idle);
	disc(renderer, &touch.attack, (holding & TMNT_ATTACK) ? held : idle);
	disc(renderer, &touch.jump, (holding & TMNT_JUMP) ? held : idle);
	disc(renderer, &touch.start, start_held ? held : idle);
	if (touch.have_font)
	{
		const int scale = label_scale(&touch.attack, "ATTACK");    /* (JUMP's the same as ATTACK's) */

		label(renderer, &touch.attack, "ATTACK", scale);
		label(renderer, &touch.jump, "JUMP", scale);
		label(renderer, &touch.start, START_LABEL, label_scale(&touch.start, START_LABEL));
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
