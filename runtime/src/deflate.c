/**
 *    Name:        deflate.c
 *    Description: a zlib stream at zlib's level 9, byte for byte the one zlib makes
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    The PNGs the first run writes (extract_graphics.c) have to be the files tools/extract_assets.py writes, whose
 *    pixels Python's zlib.compress(data, 9) packs: zlib's deflate with its defaults (a 32 KB window, memLevel 8,
 *    the default strategy) at level 9, all the input at once and Z_FINISH. This is that algorithm, step for step as
 *    zlib 1.2.13 takes it (deflate.c: fill_window, longest_match, deflate_slow; trees.c: the blocks' Huffman trees,
 *    the choice of a stored, static or dynamic block), so the bits come out the same: the window slides as zlib's
 *    does and the match search reads the same bytes past the input. zlib is Jean-loup Gailly's and Mark Adler's
 *    (zlib license); nothing of its code is copied here, only what it does.
 */
#include "deflate.h"

#include <stdlib.h>
#include <string.h>

#define W_BITS          15
#define W_SIZE          (1u << W_BITS)
#define W_MASK          (W_SIZE - 1u)
#define WINDOW_SIZE     (2u * W_SIZE)
#define HASH_BITS       15                  /* memLevel 8 + 7 */
#define HASH_SIZE       (1u << HASH_BITS)
#define HASH_MASK       (HASH_SIZE - 1u)
#define HASH_SHIFT      ((HASH_BITS + MIN_MATCH - 1) / MIN_MATCH)
#define LIT_BUFSIZE     (1u << (8 + 6))     /* 1 << (memLevel + 6) */
#define SYM_END         ((LIT_BUFSIZE - 1u) * 3u)
#define MIN_MATCH       3
#define MAX_MATCH       258
#define MIN_LOOKAHEAD   (MAX_MATCH + MIN_MATCH + 1)
#define MAX_DIST        (W_SIZE - MIN_LOOKAHEAD)
#define WIN_INIT        MAX_MATCH
#define TOO_FAR         4096
#define NIL             0
/* level 9 */
#define GOOD_MATCH      32
#define MAX_LAZY        258
#define NICE_MATCH      258
#define MAX_CHAIN       4096

#define LENGTH_CODES    29
#define LITERALS        256
#define L_CODES         (LITERALS + 1 + LENGTH_CODES)
#define D_CODES         30
#define BL_CODES        19
#define HEAP_SIZE       (2 * L_CODES + 1)
#define MAX_BITS        15
#define MAX_BL_BITS     7
#define END_BLOCK       256
#define REP_3_6         16
#define REPZ_3_10       17
#define REPZ_11_138     18
#define SMALLEST        1

/* a tree's node: its frequency or its code, its father or its length (zlib's ct_data, the same sharing) */
typedef struct
{
	uint16_t freq_code;
	uint16_t dad_len;
} node;

typedef struct
{
	node *tree;
	int max_code;
	const node *static_tree;
	const int *extra_bits;
	int extra_base, elements, max_length;
} tree_desc;

typedef struct
{
	const uint8_t *input;
	size_t input_size, input_at;
	uint8_t *window;
	uint16_t *prev, *head;
	uint32_t ins_h;
	uint32_t strstart, match_start, lookahead, insert, prev_match;
	uint32_t match_length, prev_length, match_available;
	long block_start;
	uint32_t high_water;
	/* the trees */
	node dyn_ltree[HEAP_SIZE], dyn_dtree[2 * D_CODES + 1], bl_tree[2 * BL_CODES + 1];
	tree_desc l_desc, d_desc, bl_desc;
	uint16_t bl_count[MAX_BITS + 1];
	int heap[2 * L_CODES + 1];
	int heap_len, heap_max;
	uint8_t depth[2 * L_CODES + 1];
	uint8_t *sym_buf;
	uint32_t sym_next;
	uint64_t opt_len, static_len;
	/* the output */
	uint8_t *out;
	size_t out_size, out_capacity;
	uint64_t bits;
	int bit_count;
	int failed;
} deflater;

static const int extra_lbits[LENGTH_CODES] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
static const int extra_dbits[D_CODES] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
static const int extra_blbits[BL_CODES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 7 };
static const uint8_t bl_order[BL_CODES] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

static node static_ltree[L_CODES + 2];
static node static_dtree[D_CODES];
static uint8_t dist_code[512];
static uint8_t length_code[MAX_MATCH - MIN_MATCH + 1];
static int base_length[LENGTH_CODES];
static int base_dist[D_CODES];
static int statics_ready;

/* ---- the output, its bits from the lowest ---- */

static void put_byte(deflater *s, uint8_t byte)
{
	if (s->out_size == s->out_capacity)
	{
		uint8_t *grown;

		s->out_capacity = s->out_capacity * 2 + 1024;
		if (!(grown = realloc(s->out, s->out_capacity)))
		{
			s->failed = 1;
			s->out_size = 0;
			return;
		}
		s->out = grown;
	}
	s->out[s->out_size++] = byte;
}

static void send_bits(deflater *s, uint32_t value, int length)
{
	s->bits |= (uint64_t)value << s->bit_count;
	s->bit_count += length;
	while (s->bit_count >= 8)
	{
		put_byte(s, (uint8_t)s->bits);
		s->bits >>= 8;
		s->bit_count -= 8;
	}
}

/* the bits up to the next byte, zeros (zlib's bi_windup) */
static void windup(deflater *s)
{
	if (s->bit_count > 0)
		put_byte(s, (uint8_t)s->bits);
	s->bits = 0;
	s->bit_count = 0;
}

/* ---- the trees ---- */

static uint32_t bi_reverse(uint32_t code, int length)
{
	uint32_t result = 0;

	do
	{
		result |= code & 1u;
		code >>= 1;
		result <<= 1;
	} while (--length > 0);
	return result >> 1;
}

static void gen_codes(node *tree, int max_code, const uint16_t *bl_count)
{
	uint16_t next_code[MAX_BITS + 1];
	uint32_t code = 0;

	for (int bits = 1; bits <= MAX_BITS; bits++)
	{
		code = (code + bl_count[bits - 1]) << 1;
		next_code[bits] = (uint16_t)code;
	}
	for (int n = 0; n <= max_code; n++)
	{
		const int length = tree[n].dad_len;

		if (length == 0)
			continue;
		tree[n].freq_code = (uint16_t)bi_reverse(next_code[length]++, length);
	}
}

static void static_init(void)
{
	uint16_t bl_count[MAX_BITS + 1] = { 0 };
	int length = 0, code, n, dist;

	if (statics_ready)
		return;
	for (code = 0; code < LENGTH_CODES - 1; code++)
	{
		base_length[code] = length;
		for (n = 0; n < (1 << extra_lbits[code]); n++)
			length_code[length++] = (uint8_t)code;
	}
	length_code[length - 1] = (uint8_t)code;    /* (the length 258 as code 285) */
	dist = 0;
	for (code = 0; code < 16; code++)
	{
		base_dist[code] = dist;
		for (n = 0; n < (1 << extra_dbits[code]); n++)
			dist_code[dist++] = (uint8_t)code;
	}
	dist >>= 7;
	for (; code < D_CODES; code++)
	{
		base_dist[code] = dist << 7;
		for (n = 0; n < (1 << (extra_dbits[code] - 7)); n++)
			dist_code[256 + dist++] = (uint8_t)code;
	}
	for (n = 0; n <= 143; n++, bl_count[8]++)
		static_ltree[n].dad_len = 8;
	for (; n <= 255; n++, bl_count[9]++)
		static_ltree[n].dad_len = 9;
	for (; n <= 279; n++, bl_count[7]++)
		static_ltree[n].dad_len = 7;
	for (; n <= 287; n++, bl_count[8]++)
		static_ltree[n].dad_len = 8;
	gen_codes(static_ltree, L_CODES + 1, bl_count);
	for (n = 0; n < D_CODES; n++)
	{
		static_dtree[n].dad_len = 5;
		static_dtree[n].freq_code = (uint16_t)bi_reverse((uint32_t)n, 5);
	}
	statics_ready = 1;
}

static int d_code(uint32_t dist)
{
	return dist < 256 ? dist_code[dist] : dist_code[256 + (dist >> 7)];
}

static void init_block(deflater *s)
{
	for (int n = 0; n < L_CODES; n++)
		s->dyn_ltree[n].freq_code = 0;
	for (int n = 0; n < D_CODES; n++)
		s->dyn_dtree[n].freq_code = 0;
	for (int n = 0; n < BL_CODES; n++)
		s->bl_tree[n].freq_code = 0;
	s->dyn_ltree[END_BLOCK].freq_code = 1;
	s->opt_len = s->static_len = 0;
	s->sym_next = 0;
}

static int smaller(const node *tree, int n, int m, const uint8_t *depth)
{
	return tree[n].freq_code < tree[m].freq_code || (tree[n].freq_code == tree[m].freq_code && depth[n] <= depth[m]);
}

static void pqdownheap(deflater *s, const node *tree, int k)
{
	const int v = s->heap[k];
	int j = k << 1;

	while (j <= s->heap_len)
	{
		if (j < s->heap_len && smaller(tree, s->heap[j + 1], s->heap[j], s->depth))
			j++;
		if (smaller(tree, v, s->heap[j], s->depth))
			break;
		s->heap[k] = s->heap[j];
		k = j;
		j <<= 1;
	}
	s->heap[k] = v;
}

static void gen_bitlen(deflater *s, tree_desc *desc)
{
	node *tree = desc->tree;
	const int max_code = desc->max_code, base = desc->extra_base, max_length = desc->max_length;
	int h, n, m, bits, overflow = 0;

	for (bits = 0; bits <= MAX_BITS; bits++)
		s->bl_count[bits] = 0;
	tree[s->heap[s->heap_max]].dad_len = 0;
	for (h = s->heap_max + 1; h < HEAP_SIZE; h++)
	{
		int xbits = 0;
		uint16_t frequency;

		n = s->heap[h];
		bits = tree[tree[n].dad_len].dad_len + 1;
		if (bits > max_length)
		{
			bits = max_length;
			overflow++;
		}
		tree[n].dad_len = (uint16_t)bits;
		if (n > max_code)
			continue;
		s->bl_count[bits]++;
		if (n >= base)
			xbits = desc->extra_bits[n - base];
		frequency = tree[n].freq_code;
		s->opt_len += (uint64_t)frequency * (uint64_t)(bits + xbits);
		if (desc->static_tree)
			s->static_len += (uint64_t)frequency * (uint64_t)(desc->static_tree[n].dad_len + xbits);
	}
	if (overflow == 0)
		return;
	do
	{
		bits = max_length - 1;
		while (s->bl_count[bits] == 0)
			bits--;
		s->bl_count[bits]--;
		s->bl_count[bits + 1] += 2;
		s->bl_count[max_length]--;
		overflow -= 2;
	} while (overflow > 0);
	for (bits = max_length; bits != 0; bits--)
	{
		n = s->bl_count[bits];
		while (n != 0)
		{
			m = s->heap[--h];
			if (m > max_code)
				continue;
			if (tree[m].dad_len != (uint32_t)bits)
			{
				s->opt_len = (uint64_t)((int64_t)s->opt_len + ((int64_t)bits - tree[m].dad_len) * (int64_t)tree[m].freq_code);
				tree[m].dad_len = (uint16_t)bits;
			}
			n--;
		}
	}
}

static void build_tree(deflater *s, tree_desc *desc)
{
	node *tree = desc->tree;
	const node *stree = desc->static_tree;
	const int elements = desc->elements;
	int n, m, max_code = -1, next;

	s->heap_len = 0;
	s->heap_max = HEAP_SIZE;
	for (n = 0; n < elements; n++)
		if (tree[n].freq_code != 0)
		{
			s->heap[++s->heap_len] = max_code = n;
			s->depth[n] = 0;
		}
		else
			tree[n].dad_len = 0;
	while (s->heap_len < 2)
	{
		next = s->heap[++s->heap_len] = max_code < 2 ? ++max_code : 0;
		tree[next].freq_code = 1;
		s->depth[next] = 0;
		s->opt_len--;
		if (stree)
			s->static_len -= stree[next].dad_len;
	}
	desc->max_code = max_code;
	for (n = s->heap_len / 2; n >= 1; n--)
		pqdownheap(s, tree, n);
	next = elements;
	do
	{
		n = s->heap[SMALLEST];
		s->heap[SMALLEST] = s->heap[s->heap_len--];
		pqdownheap(s, tree, SMALLEST);
		m = s->heap[SMALLEST];
		s->heap[--s->heap_max] = n;
		s->heap[--s->heap_max] = m;
		tree[next].freq_code = (uint16_t)(tree[n].freq_code + tree[m].freq_code);
		s->depth[next] = (uint8_t)((s->depth[n] >= s->depth[m] ? s->depth[n] : s->depth[m]) + 1);
		tree[n].dad_len = tree[m].dad_len = (uint16_t)next;
		s->heap[SMALLEST] = next++;
		pqdownheap(s, tree, SMALLEST);
	} while (s->heap_len >= 2);
	s->heap[--s->heap_max] = s->heap[SMALLEST];
	gen_bitlen(s, desc);
	gen_codes(tree, max_code, s->bl_count);
}

static void scan_tree(deflater *s, node *tree, int max_code)
{
	int prevlen = -1, curlen, nextlen = tree[0].dad_len, count = 0, max_count = 7, min_count = 4;

	if (nextlen == 0)
	{
		max_count = 138;
		min_count = 3;
	}
	tree[max_code + 1].dad_len = 0xFFFF;    /* (the guard) */
	for (int n = 0; n <= max_code; n++)
	{
		curlen = nextlen;
		nextlen = tree[n + 1].dad_len;
		if (++count < max_count && curlen == nextlen)
			continue;
		else if (count < min_count)
			s->bl_tree[curlen].freq_code = (uint16_t)(s->bl_tree[curlen].freq_code + count);
		else if (curlen != 0)
		{
			if (curlen != prevlen)
				s->bl_tree[curlen].freq_code++;
			s->bl_tree[REP_3_6].freq_code++;
		}
		else if (count <= 10)
			s->bl_tree[REPZ_3_10].freq_code++;
		else
			s->bl_tree[REPZ_11_138].freq_code++;
		count = 0;
		prevlen = curlen;
		if (nextlen == 0)
		{
			max_count = 138;
			min_count = 3;
		}
		else if (curlen == nextlen)
		{
			max_count = 6;
			min_count = 3;
		}
		else
		{
			max_count = 7;
			min_count = 4;
		}
	}
}

static void send_code(deflater *s, int code, const node *tree)
{
	send_bits(s, tree[code].freq_code, tree[code].dad_len);
}

static void send_tree(deflater *s, const node *tree, int max_code)
{
	int prevlen = -1, curlen, nextlen = tree[0].dad_len, count = 0, max_count = 7, min_count = 4;

	if (nextlen == 0)
	{
		max_count = 138;
		min_count = 3;
	}
	for (int n = 0; n <= max_code; n++)
	{
		curlen = nextlen;
		nextlen = tree[n + 1].dad_len;
		if (++count < max_count && curlen == nextlen)
			continue;
		else if (count < min_count)
		{
			do
				send_code(s, curlen, s->bl_tree);
			while (--count != 0);
		}
		else if (curlen != 0)
		{
			if (curlen != prevlen)
			{
				send_code(s, curlen, s->bl_tree);
				count--;
			}
			send_code(s, REP_3_6, s->bl_tree);
			send_bits(s, (uint32_t)(count - 3), 2);
		}
		else if (count <= 10)
		{
			send_code(s, REPZ_3_10, s->bl_tree);
			send_bits(s, (uint32_t)(count - 3), 3);
		}
		else
		{
			send_code(s, REPZ_11_138, s->bl_tree);
			send_bits(s, (uint32_t)(count - 11), 7);
		}
		count = 0;
		prevlen = curlen;
		if (nextlen == 0)
		{
			max_count = 138;
			min_count = 3;
		}
		else if (curlen == nextlen)
		{
			max_count = 6;
			min_count = 3;
		}
		else
		{
			max_count = 7;
			min_count = 4;
		}
	}
}

static int build_bl_tree(deflater *s)
{
	int max_blindex;

	scan_tree(s, s->dyn_ltree, s->l_desc.max_code);
	scan_tree(s, s->dyn_dtree, s->d_desc.max_code);
	build_tree(s, &s->bl_desc);
	for (max_blindex = BL_CODES - 1; max_blindex >= 3; max_blindex--)
		if (s->bl_tree[bl_order[max_blindex]].dad_len != 0)
			break;
	s->opt_len += 3u * ((uint64_t)max_blindex + 1u) + 5u + 5u + 4u;
	return max_blindex;
}

static void send_all_trees(deflater *s, int lcodes, int dcodes, int blcodes)
{
	send_bits(s, (uint32_t)(lcodes - 257), 5);
	send_bits(s, (uint32_t)(dcodes - 1), 5);
	send_bits(s, (uint32_t)(blcodes - 4), 4);
	for (int rank = 0; rank < blcodes; rank++)
		send_bits(s, s->bl_tree[bl_order[rank]].dad_len, 3);
	send_tree(s, s->dyn_ltree, lcodes - 1);
	send_tree(s, s->dyn_dtree, dcodes - 1);
}

static void compress_block(deflater *s, const node *ltree, const node *dtree)
{
	uint32_t at = 0;

	while (at < s->sym_next)
	{
		uint32_t dist = s->sym_buf[at] | (uint32_t)s->sym_buf[at + 1] << 8;
		int lc = s->sym_buf[at + 2];

		at += 3;
		if (dist == 0)
			send_code(s, lc, ltree);
		else
		{
			int code = length_code[lc], extra = extra_lbits[code];

			send_code(s, code + LITERALS + 1, ltree);
			if (extra != 0)
				send_bits(s, (uint32_t)(lc - base_length[code]), extra);
			dist--;
			code = d_code(dist);
			send_code(s, code, dtree);
			extra = extra_dbits[code];
			if (extra != 0)
				send_bits(s, dist - (uint32_t)base_dist[code], extra);
		}
	}
	send_code(s, END_BLOCK, ltree);
}

static void stored_block(deflater *s, const uint8_t *buffer, uint32_t stored_len, int last)
{
	send_bits(s, (uint32_t)last, 3);    /* (STORED_BLOCK << 1) + last */
	windup(s);
	put_byte(s, (uint8_t)stored_len);
	put_byte(s, (uint8_t)(stored_len >> 8));
	put_byte(s, (uint8_t)~stored_len);
	put_byte(s, (uint8_t)(~stored_len >> 8));
	for (uint32_t index = 0; index < stored_len; index++)
		put_byte(s, buffer[index]);
}

static void flush_block(deflater *s, int last)
{
	const uint8_t *buffer = s->block_start >= 0 ? &s->window[s->block_start] : NULL;
	const uint32_t stored_len = (uint32_t)((long)s->strstart - s->block_start);
	uint64_t opt_lenb, static_lenb;
	int max_blindex;

	build_tree(s, &s->l_desc);
	build_tree(s, &s->d_desc);
	max_blindex = build_bl_tree(s);
	opt_lenb = (s->opt_len + 3u + 7u) >> 3;
	static_lenb = (s->static_len + 3u + 7u) >> 3;
	if (static_lenb <= opt_lenb)
		opt_lenb = static_lenb;
	if ((uint64_t)stored_len + 4u <= opt_lenb && buffer)
		stored_block(s, buffer, stored_len, last);
	else if (static_lenb == opt_lenb)
	{
		send_bits(s, (uint32_t)((1 << 1) + last), 3);    /* STATIC_TREES */
		compress_block(s, static_ltree, static_dtree);
	}
	else
	{
		send_bits(s, (uint32_t)((2 << 1) + last), 3);    /* DYN_TREES */
		send_all_trees(s, s->l_desc.max_code + 1, s->d_desc.max_code + 1, max_blindex + 1);
		compress_block(s, s->dyn_ltree, s->dyn_dtree);
	}
	init_block(s);
	if (last)
		windup(s);
	s->block_start = (long)s->strstart;
}

/* ---- the matches ---- */

#define UPDATE_HASH(s, c) ((s)->ins_h = (((s)->ins_h << HASH_SHIFT) ^ (c)) & HASH_MASK)

static uint32_t insert_string(deflater *s, uint32_t at)
{
	uint32_t match_head;

	UPDATE_HASH(s, s->window[at + (MIN_MATCH - 1)]);
	match_head = s->prev[at & W_MASK] = s->head[s->ins_h];
	s->head[s->ins_h] = (uint16_t)at;
	return match_head;
}

static void slide_hash(deflater *s)
{
	for (uint32_t n = 0; n < HASH_SIZE; n++)
		s->head[n] = (uint16_t)(s->head[n] >= W_SIZE ? s->head[n] - W_SIZE : NIL);
	for (uint32_t n = 0; n < W_SIZE; n++)
		s->prev[n] = (uint16_t)(s->prev[n] >= W_SIZE ? s->prev[n] - W_SIZE : NIL);
}

static void fill_window(deflater *s)
{
	uint32_t more;

	do
	{
		more = WINDOW_SIZE - s->lookahead - s->strstart;
		if (s->strstart >= W_SIZE + MAX_DIST)
		{
			memcpy(s->window, s->window + W_SIZE, W_SIZE - more);
			s->match_start -= W_SIZE;
			s->strstart -= W_SIZE;
			s->block_start -= (long)W_SIZE;
			if (s->insert > s->strstart)
				s->insert = s->strstart;
			slide_hash(s);
			more += W_SIZE;
		}
		if (s->input_at == s->input_size)
			break;
		{
			size_t count = s->input_size - s->input_at;

			if (count > more)
				count = more;
			memcpy(s->window + s->strstart + s->lookahead, s->input + s->input_at, count);
			s->input_at += count;
			s->lookahead += (uint32_t)count;
		}
		if (s->lookahead + s->insert >= MIN_MATCH)
		{
			uint32_t at = s->strstart - s->insert;

			s->ins_h = s->window[at];
			UPDATE_HASH(s, s->window[at + 1]);
			while (s->insert)
			{
				UPDATE_HASH(s, s->window[at + MIN_MATCH - 1]);
				s->prev[at & W_MASK] = s->head[s->ins_h];
				s->head[s->ins_h] = (uint16_t)at;
				at++;
				s->insert--;
				if (s->lookahead + s->insert < MIN_MATCH)
					break;
			}
		}
	} while (s->lookahead < MIN_LOOKAHEAD && s->input_at != s->input_size);
	/* the bytes past the input zeroed, as zlib's high water mark has them */
	if (s->high_water < WINDOW_SIZE)
	{
		const uint32_t current = s->strstart + s->lookahead;
		uint32_t init;

		if (s->high_water < current)
		{
			init = WINDOW_SIZE - current;
			if (init > WIN_INIT)
				init = WIN_INIT;
			memset(s->window + current, 0, init);
			s->high_water = current + init;
		}
		else if (s->high_water < current + WIN_INIT)
		{
			init = current + WIN_INIT - s->high_water;
			if (init > WINDOW_SIZE - s->high_water)
				init = WINDOW_SIZE - s->high_water;
			memset(s->window + s->high_water, 0, init);
			s->high_water += init;
		}
	}
}

static uint32_t longest_match(deflater *s, uint32_t cur_match)
{
	uint32_t chain_length = MAX_CHAIN;
	const uint8_t *scan = s->window + s->strstart;
	const uint8_t *match;
	int length, best_len = (int)s->prev_length, nice_match = NICE_MATCH;
	const uint32_t limit = s->strstart > MAX_DIST ? s->strstart - MAX_DIST : NIL;
	const uint8_t *strend = s->window + s->strstart + MAX_MATCH;
	uint8_t scan_end1 = scan[best_len - 1], scan_end = scan[best_len];

	if (s->prev_length >= GOOD_MATCH)
		chain_length >>= 2;
	if ((uint32_t)nice_match > s->lookahead)
		nice_match = (int)s->lookahead;
	do
	{
		match = s->window + cur_match;
		if (match[best_len] != scan_end || match[best_len - 1] != scan_end1 || *match != *scan || *++match != scan[1])
			continue;
		scan += 2;
		match++;
		do
		{
		} while (*++scan == *++match && *++scan == *++match && *++scan == *++match && *++scan == *++match
			&& *++scan == *++match && *++scan == *++match && *++scan == *++match && *++scan == *++match && scan < strend);
		length = MAX_MATCH - (int)(strend - scan);
		scan = strend - MAX_MATCH;
		if (length > best_len)
		{
			s->match_start = cur_match;
			best_len = length;
			if (length >= nice_match)
				break;
			scan_end1 = scan[best_len - 1];
			scan_end = scan[best_len];
		}
	} while ((cur_match = s->prev[cur_match & W_MASK]) > limit && --chain_length != 0);
	return (uint32_t)best_len <= s->lookahead ? (uint32_t)best_len : s->lookahead;
}

static void tally(deflater *s, uint32_t dist, uint32_t lc, int *flush)
{
	s->sym_buf[s->sym_next++] = (uint8_t)dist;
	s->sym_buf[s->sym_next++] = (uint8_t)(dist >> 8);
	s->sym_buf[s->sym_next++] = (uint8_t)lc;
	if (dist == 0)
		s->dyn_ltree[lc].freq_code++;
	else
	{
		s->dyn_ltree[length_code[lc] + LITERALS + 1].freq_code++;
		s->dyn_dtree[d_code(dist - 1)].freq_code++;
	}
	*flush = s->sym_next == SYM_END;
}

/* deflate_slow with Z_FINISH */
static void deflate_slow(deflater *s)
{
	uint32_t hash_head;
	int flush;

	for (;;)
	{
		if (s->lookahead < MIN_LOOKAHEAD)
		{
			fill_window(s);
			if (s->lookahead == 0)
				break;
		}
		hash_head = NIL;
		if (s->lookahead >= MIN_MATCH)
			hash_head = insert_string(s, s->strstart);
		s->prev_length = s->match_length;
		s->prev_match = s->match_start;
		s->match_length = MIN_MATCH - 1;
		if (hash_head != NIL && s->prev_length < MAX_LAZY && s->strstart - hash_head <= MAX_DIST)
		{
			s->match_length = longest_match(s, hash_head);
			if (s->match_length <= 5 && s->match_length == MIN_MATCH && s->strstart - s->match_start > TOO_FAR)
				s->match_length = MIN_MATCH - 1;
		}
		if (s->prev_length >= MIN_MATCH && s->match_length <= s->prev_length)
		{
			const uint32_t max_insert = s->strstart + s->lookahead - MIN_MATCH;

			tally(s, s->strstart - 1 - s->prev_match, s->prev_length - MIN_MATCH, &flush);
			s->lookahead -= s->prev_length - 1;
			s->prev_length -= 2;
			do
			{
				if (++s->strstart <= max_insert)
					insert_string(s, s->strstart);
			} while (--s->prev_length != 0);
			s->match_available = 0;
			s->match_length = MIN_MATCH - 1;
			s->strstart++;
			if (flush)
				flush_block(s, 0);
		}
		else if (s->match_available)
		{
			tally(s, 0, s->window[s->strstart - 1], &flush);
			if (flush)
				flush_block(s, 0);
			s->strstart++;
			s->lookahead--;
		}
		else
		{
			s->match_available = 1;
			s->strstart++;
			s->lookahead--;
		}
	}
	if (s->match_available)
	{
		tally(s, 0, s->window[s->strstart - 1], &flush);
		s->match_available = 0;
	}
	s->insert = s->strstart < MIN_MATCH - 1 ? s->strstart : MIN_MATCH - 1;
	flush_block(s, 1);
}

static uint32_t adler32(const uint8_t *data, size_t size)
{
	uint32_t low = 1, high = 0;

	for (size_t index = 0; index < size; index++)
	{
		low = (low + data[index]) % 65521u;
		high = (high + low) % 65521u;
	}
	return high << 16 | low;
}

uint8_t *deflate_zlib(const uint8_t *data, size_t size, size_t *packed)
{
	deflater *s = calloc(1, sizeof(*s));
	uint8_t *out;
	uint32_t check;

	if (!s)
		return NULL;
	static_init();
	s->window = calloc(WINDOW_SIZE, 1);
	s->prev = calloc(W_SIZE, sizeof(uint16_t));
	s->head = calloc(HASH_SIZE, sizeof(uint16_t));
	s->sym_buf = malloc(LIT_BUFSIZE * 3u);
	s->out_capacity = size / 2u + 1024u;
	s->out = malloc(s->out_capacity);
	if (!s->window || !s->prev || !s->head || !s->sym_buf || !s->out)
		s->failed = 1;
	if (!s->failed)
	{
		s->input = data;
		s->input_size = size;
		s->l_desc = (tree_desc){ s->dyn_ltree, 0, static_ltree, extra_lbits, LITERALS + 1, L_CODES, MAX_BITS };
		s->d_desc = (tree_desc){ s->dyn_dtree, 0, static_dtree, extra_dbits, 0, D_CODES, MAX_BITS };
		s->bl_desc = (tree_desc){ s->bl_tree, 0, NULL, extra_blbits, 0, BL_CODES, MAX_BL_BITS };
		init_block(s);
		s->match_length = s->prev_length = MIN_MATCH - 1;
		/* the header: deflate, a 32 KB window, level 9's flags (3), its check */
		put_byte(s, 0x78);
		put_byte(s, 0xDA);
		deflate_slow(s);
		check = adler32(data, size);
		put_byte(s, (uint8_t)(check >> 24));
		put_byte(s, (uint8_t)(check >> 16));
		put_byte(s, (uint8_t)(check >> 8));
		put_byte(s, (uint8_t)check);
	}
	free(s->window);
	free(s->prev);
	free(s->head);
	free(s->sym_buf);
	out = s->failed ? NULL : s->out;
	if (s->failed)
		free(s->out);
	else
		*packed = s->out_size;
	free(s);
	return out;
}
