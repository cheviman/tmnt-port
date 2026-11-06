/**
 *    Name:        touch.c
 *    Description: the touch screen's controls (Android): a stick where the left thumb lands, ATTACK and JUMP at the
 *                 right, START (TMNT) or COIN (TMNT Arcade) over them, drawn over the picture, and what the fingers
 *                 on them hold: player 1's controls
 *    Version:     0.1.0
 *    Created:     06-11-2026
 *    Modified:    06-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    As outrunners-port's (its wheel and pedals for a car; here the cabinet's stick and two buttons). A finger that
 *    lands on the screen's left half is the stick: its centre where it landed, its way the finger's from there (eight,
 *    as the cabinet's); one on a button holds it, and sliding onto the other holds that one. The controls are drawn
 *    from where the screen's sides are, a size of its height, over the picture's corners; a game controller or a
 *    key hides them till a finger touches the screen again. The letters are font.bmp's (Kenney Mini, ports/sdl2/font).
 */
#include "touch.h"

#include "letters.h"
#include "tmnt/machine.h"

#include <math.h>
#include <string.h>

#define FINGERS             10
#define STICK_DEAD          0.30       /* of the stick's reach: no way within it */
#define STICK_DIAGONAL      0.38       /* the share of the way's other axis that makes it a diagonal (cos 67.5) */

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
	circle stick_home, attack, jump, start;
	float stick_reach;
	struct
	{
		SDL_FingerID id;
		int holds;
		float x0, y0, x, y;            /* (the stick's: where it landed, where it is) */
	} fingers[FINGERS];
} touch;

/* Where the controls go on a screen of 'width' x 'height': a size of its height, from its sides */
static void place(int width, int height)
{
	const float unit = (float)height / 10.0f;

	if (width == touch.width && height == touch.height)
		return;
	touch.width = width;
	touch.height = height;
	touch.stick_reach = unit * 1.3f;
	touch.stick_home = (circle){ unit * 2.2f, (float)height - unit * 2.4f, touch.stick_reach };
	touch.attack = (circle){ (float)width - unit * 1.6f, (float)height - unit * 2.0f, unit * 1.05f };
	touch.jump = (circle){ (float)width - unit * 3.9f, (float)height - unit * 1.4f, unit * 1.05f };
	touch.start = (circle){ (float)width - unit * 1.6f, unit * 1.3f, unit * 0.7f };
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
	touch.have_font = letters_load(renderer, base, &touch.font) != 0;
	touch.shown = SDL_GetNumTouchDevices() > 0;
}

void touch_event(const SDL_Event *event)
{
	switch (event->type)
	{
		case SDL_FINGERDOWN:
		case SDL_FINGERMOTION:
		case SDL_FINGERUP:
		{
			const float x = event->tfinger.x * (float)touch.width, y = event->tfinger.y * (float)touch.height;
			int index = -1, free_one = -1;

			touch.shown = true;
			for (int finger = 0; finger < FINGERS; finger++)
			{
				if (touch.fingers[finger].holds != HOLD_NOTHING && touch.fingers[finger].id == event->tfinger.fingerId)
					index = finger;
				else if (touch.fingers[finger].holds == HOLD_NOTHING && free_one < 0)
					free_one = finger;
			}
			if (event->type == SDL_FINGERUP)
			{
				if (index >= 0)
					touch.fingers[index].holds = HOLD_NOTHING;
				break;
			}
			if (index < 0)
			{
				if (event->type != SDL_FINGERDOWN || free_one < 0 || (touch.fingers[free_one].holds = under(x, y))
					== HOLD_NOTHING)
					break;
				index = free_one;
				touch.fingers[index].id = event->tfinger.fingerId;
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
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_KEYDOWN:
			touch.shown = false;
			memset(touch.fingers, 0, sizeof(touch.fingers));
			break;
		case SDL_CONTROLLERAXISMOTION:
			if (event->caxis.value > 16000 || event->caxis.value < -16000)
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
		SDL_RenderDrawLine(renderer, (int)where->x - dx, (int)where->y + dy, (int)where->x + dx, (int)where->y + dy);
		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, (Uint8)SDL_min(255, colour.a * 2));
		SDL_RenderDrawPoint(renderer, (int)where->x - dx, (int)where->y + dy);
		SDL_RenderDrawPoint(renderer, (int)where->x + dx, (int)where->y + dy);
	}
}

/* A button's name, centred on it */
static void label(SDL_Renderer *renderer, const circle *where, const char *text)
{
	const int scale = SDL_max(1, (int)(where->radius / 22.0f));
	const int width = letters_width(&touch.font, text, scale);
	const SDL_Rect area = { (int)where->x - width / 2, (int)where->y - LETTERS_CELL * scale / 2, width,
		LETTERS_CELL * scale };
	const SDL_Color white = { 255, 255, 255, 220 };

	if (!touch.have_font)
		return;
	SDL_RenderSetViewport(renderer, &area);
	letters_draw(renderer, &touch.font, text, 0, scale, white, width);
	SDL_RenderSetViewport(renderer, NULL);
}

void touch_draw(SDL_Renderer *renderer, int width, int height)
{
	const SDL_Color idle = { 255, 255, 255, 50 }, held = { 255, 230, 120, 110 };
	const uint8_t holding = touch_controls();
	const bool start_held = touch_start();
	circle stick = { 0 }, knob;

	place(width, height);
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
	label(renderer, &touch.attack, "ATTACK");
	label(renderer, &touch.jump, "JUMP");
	label(renderer, &touch.start, START_LABEL);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
