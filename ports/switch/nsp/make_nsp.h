/**
 *    Name:        make_nsp.h
 *    Description: TMNT NSP's work, for its command line and its window alike: the .nsp from its inputs,
 *                 their defaults, and what the window shows of each
 *    Version:     0.1.0
 *    Created:     06-11-2026
 *    Modified:    06-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Paths are as the C library's fopen takes them (on Windows, in the system's code page).
 */
#ifndef TMNT_NSP_MAKE_NSP_H
#define TMNT_NSP_MAKE_NSP_H

#include <stddef.h>
#include <stdio.h>

#define NSP_PATH_SIZE 1100
#define NSP_FILE      "TMNT.nsp"
#define NSP_ARCADE_FILE "TMNT Arcade.nsp"    /* (TMNT Arcade's, beside TMNT's) */
#define NSP_OWN_ICON  "icon.jpg"         /* the player's own icon, taken when it is beside the tool */

/* What the .nsp is made of, and where it goes */
typedef struct
{
	char keys[NSP_PATH_SIZE];      /* the console's keys: prod.keys */
	char rom[NSP_PATH_SIZE];       /* the arcade ROM set: tmnt.zip, or a folder of its files */
	char pieces[NSP_PATH_SIZE];    /* the games' Switch builds, the folder switch: tmnt and tmnt-arcade, each with
	                                  main, main.npdm, control.nacp, icon.jpg */
	char icon[NSP_PATH_SIZE];      /* the HOME menu's icon, a 256 x 256 JPEG of the player's; "" the build's */
	char out[NSP_PATH_SIZE];       /* TMNT's .nsp to write; TMNT Arcade's beside it */
} nsp_inputs;

/* How making it went */
typedef struct
{
	char what[80];                 /* what failed */
	char detail[NSP_PATH_SIZE + 128];    /* what it failed on (a file, or what the part that failed tells), or "" */
	size_t size;                   /* the .nsp's bytes, when it is written */
} nsp_outcome;

/* Told the step making the .nsp is at, a name of its own ("taking the assets out of the ROM set") */
typedef void (*nsp_step)(void *context, const char *step);

/* A file dropped on the tool, taken by what it is: a .keys (or .txt) the keys, a .jpg the icon, anything
   else the ROM set (tmnt.zip, or a folder of its files) */
void nsp_take_dropped(nsp_inputs *inputs, const char *path);

/* The inputs not given, beside the tool (in the folder 'here', with its separator): prod.keys (else in
   .switch in the user's folder), tmnt.zip, the folder switch, TMNT.nsp, and the player's icon.jpg
   when it is there (else the build's own icon) */
void nsp_defaults(nsp_inputs *inputs, const char *here);

/* The .nsps, TMNT's and TMNT Arcade's: true when both are written. Each step's lines go to 'log' (the command line's) unless it is NULL;
   'step' (or NULL) is told each step as it starts. What failed is in 'outcome' */
int make_nsp(const nsp_inputs *inputs, FILE *log, nsp_step step, void *context, nsp_outcome *outcome);

/* The folder of a path, with its separator: "" for a bare name */
void nsp_folder_of(const char *path, char *folder, size_t size);

/* What the window shows of each input: NULL when it is good, else what is wrong with it ("not found") */
const char *nsp_keys_problem(const char *path);
const char *nsp_icon_problem(const char *path);
int nsp_file_exists(const char *path);
int nsp_folder_exists(const char *path);

#endif
