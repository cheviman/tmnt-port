/**
 *    Name:        extract_internal.h
 *    Description: what the parts of the first run share (extract.c, extract_graphics.c, extract_samples.c,
 *                 extract_music.c)
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#ifndef TMNT_EXTRACT_INTERNAL_H
#define TMNT_EXTRACT_INTERNAL_H

#include <stddef.h>
#include <stdint.h>

#define EXTRACT_PATH_SIZE 1200

/* what failed (the parts set it) */
extern char extract_problem[EXTRACT_PATH_SIZE + 128];

/* a file's bytes or a text file's lines, made in memory */
typedef struct
{
	char *data;
	size_t size, capacity;
	int failed;
} extract_buffer;

void buffer_bytes(extract_buffer *into, const void *bytes, size_t size);
void buffer_byte(extract_buffer *into, uint8_t byte);
void buffer_text(extract_buffer *into, const char *format, ...);
void buffer_free(extract_buffer *buffer);

/* 'relative' in 'folder' written whole: a text file as Python writes one (its line ends the system's), or bytes.
   0 with the problem set when it cannot be */
int extract_write_text(const char *folder, const char *relative, const extract_buffer *text);
int extract_write_bytes(const char *folder, const char *relative, const void *bytes, size_t size);

/* the parts: 1 when their files are written */
int extract_graphics(const char *folder);
int extract_samples(const char *folder);
int extract_music(const char *folder);

#endif
