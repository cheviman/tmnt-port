/**
 *    Name:        deflate.h
 *    Description: a zlib stream at zlib's level 9, byte for byte the one zlib makes (deflate.c)
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 */
#ifndef TMNT_DEFLATE_H
#define TMNT_DEFLATE_H

#include <stddef.h>
#include <stdint.h>

/* 'size' bytes of 'data' as zlib's compress2(level 9) gives them (its 2-byte header, the DEFLATE blocks, the
   Adler-32): a buffer the caller frees and its size in '*packed', or NULL when there is no memory */
uint8_t *deflate_zlib(const uint8_t *data, size_t size, size_t *packed);

#endif
