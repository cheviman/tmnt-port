"""
Name:        data_walk.py
Description: the program's data the code hands to its engines, walked from where it hands it over: the
             animations the sprite engine plays, the sprite lists and boxes of their frames
Version:     0.1.0
Created:     29-10-2026
Modified:    04-11-2026
Author:      Sebastian Toyos

Usage: python tools/data_walk.py [rom folder or .zip] [--reads FOLDER]
Writes build/listing/data_walk.txt.

The static analysis (tools/core/m68000/analysis.py) says what the address registers can hold where a
function starts; the engines take their data there, so what a register holds on the way into one is where
that data starts (no traces: they only measure the walk). From each start the walk reads the data as the
engine does:
  animation      frames one after the other (set_animation, 047BF2, takes the first in A6; the sprite
                 engine steps through them, 04711E and 0471BE): a byte of frames to show it, the number of
                 sprites less one, the address of its sprite list, the offset of its body box, and an
                 attack box (+8: 8 bytes from there, or a negative word when there is none: the frame is 10
                 bytes then, 18 with one). A first frame with a 0 count is a still: the engine never steps
                 on from it (what follows is something else). After the first frame, a frame whose first
                 byte is 0 ends the animation: with a 0 after it, it stays on
                 the frame before (2 bytes); else a 32-bit address follows, the frame it goes on from (6
                 bytes), walked as an animation too.
  animation table  the addresses of animations the code picks one from (a table it reads by an index into
                 A6 before set_animation), up to the first entry that is no animation or the next thing
                 walked.
  sprite list    8 bytes a sprite (draw_sprites, 046F92, writes them into the K051960's sprite RAM): the
                 sprite's size (top 3 bits) and code, its colour, its flips (bit 7 across, bit 6 down), and
                 its x and y from the object.
  body box       4 bytes in body_boxes (053756), at the offset a frame gives: half its width and its
                 height, as the collision code reads them (047204, 047534, 047662).
The walk is right when every byte the engines' instructions read in the scenarios is in what it walked:
with --reads (the reference's rom_reads.txt, kref --rom-reads) the report lists the ones that are not.
"""
import sys
from bisect import bisect_right
from collections import Counter, defaultdict
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path[:0] = [str(TOOLS / "core"), str(TOOLS)]
import game_profile  # noqa: E402  (the game's: its root, its ROM set)
from m68000 import analysis as m68000_analysis  # noqa: E402

ROOT = game_profile.ROOT
SHADOW_FIELD = 0x3A             # an object's single sprite (its shadow)
FRAME = 10                      # a frame without an attack box; ATTACK_FRAME with one
ATTACK_FRAME = 18
SPRITE = 8
SPRITE_REACH = 0x200            # a sprite's x and y from its object are within this
MAX_BOX = 0x400                 # a frame's body box offset is under this (body_boxes is 0x308 bytes)
MAX_FRAMES = 256                # an animation longer than this is no animation
MAX_TABLE = 1024                # a table of more entries than this is none
# the tables of the tile blocks objects draw (symbols.txt: their data= fields are the blocks)
TILE_BLOCK_TABLES = ("object_tile_blocks", "object_tile_block_pairs")
# what the walk takes from symbols.txt: set_animation (the animation it starts is in A6), sprite_sizes (8 of
# 8 bytes, by the top 3 bits of a sprite's code), body_boxes (4-byte records, by a frame's offset)
NAMES = ("set_animation", "sprite_sizes", "body_boxes", "area_layers", "post_palette_request", "copy_palette_sets",
         "area_palettes",
         "unused_animation_list", "palette_bank_counts", "area_spawn_lists", "spawn_offsets", "area_spawn_scrolls", "area_spawn_starts",
         "fire_tiles_area_0", "fire_tiles", "fire_blocks", "start_palette_effects", "draw_tile_block",
         "tile_block_places") + TILE_BLOCK_TABLES
# a tile block (draw_tile_block: A6 the block, D7 where, by tile_block_places' addresses in the tile RAM): its
# width and height less one (a byte each), then its tiles a row at a time, 2 bytes each (code, colour)
MAX_TILE_BLOCK = (64, 32)
# the texts (print_game_text, print_demo_text, print_test_text: D7 the text's number in their table; and the stages'
# titles, print_stage_title by the stage: 002604 "movea.l #$3402, A6", 00261E "movea.l (A6)+, A4"): a text
# is the address in the K052109's fixed layer it starts at (32 bits), then its characters (the tile is the
# character less 0x30: '@' is a space), '/' and the next line's address, '?' its end. A table ends where its
# first text starts
TEXT_TABLES = ("game_texts", "demo_texts", "test_mode_texts", "stage_titles")
TEXT_END, TEXT_LINE = 0x3F, 0x2F
# tables whose size the code that reads them fixes: (name in symbols.txt, bytes, kind)
FIXED = (("direction_rows", 64, "direction rows"), ("direction_table", 1024, "direction table"),
         ("text_layer_places", 16, "text layer places"), ("big_font_place", 4, "big font place"),
         ("big_font_tiles", 6, "big font tiles"), ("big_font", 20 * 36, "big font"), ("text_colours", 16, "text colours"),
         ("enemy_definitions", 9 * 136, "enemy definitions"), ("unused_enemy_definitions", 0x380, "unused enemy definitions"),
         ("area_tile_banks", 2 * 10, "area tile banks"), ("area_layer_priorities", 10, "area layer priorities"),
         ("area_entry_without_blink", 10, "area settings"),
         # the point add_point_to_score adds, two BCD digits it reads backwards from draw_score_digits' address
         # (001F92 "lea $2032.l, A5", 001F98 "move.w #$1, D7", 001FA0 "abcd -(A5), -(A4)")
         ("one_point", 2, "bcd number"),
         # the credit name the staff roll types when a pass has no name: 15 spaces and its '!' (007486 "move.l #$77d2,
         # $63434.l"; type_credit_name_pass reads it a character at a time to the '!', 007512 "move.b (A6)+, D6")
         ("blank_credit_name", 16, "credit name"),
         # the pictures draw_tv_picture puts on the shop window's TV in the street: 8 rows of 4 tiles' codes for the
         # left half and 4 for the right (01F08C "moveq #$7, D7", 01F08E "moveq #$3, D6", 01F098 "move.b (A6)+, (A0)+",
         # 01F094 "move.b ($4,A6), (A1)+", 01F0A6 "addq.l #4, A6"): 64 bytes
         ("april_tv_picture", 64, "tv picture"), ("tv_picture_tiles", 64, "tv picture"),
         # Splinter's picture after area 7: 16 columns of 8 rows, a column at a time (02D0FC "moveq #$10, D0", 02D120
         # "move.b ($0,A1), ($1,A0)" to 02D14A "move.b ($70,A1), ($381,A0)", 02D150 "addq.l #1, A1")
         ("splinter_picture_tiles", 128, "picture tiles"),
         # the ranking's frame: its top row and the row under it, 9 tiles each (008164 "moveq #$8, D5", 008184
         # "move.b (A5)+, ($1,A6,D4.w)", 008176 "move.b ($9,A5), ($1,A6,D4.w)")
         ("ranking_frame_tiles", 18, "picture tiles"),
         # the Technodrome's wreck: two parts of 8 bytes (007230 "lea $7276.l, A6", 007240 "move.b ($6,A6), D6", 00726A
         # "adda.l #$8, A6", 007270 "dbra D7, $7236" with D7 1)
         ("technodrome_wreck_parts", 16, "wreck parts"),
         # the contacts the floor cells FD, FE and FF clear, read back from 04881B by the cell (0487FE "lea $4881b.l, A5",
         # 048804 "move.b (A5,D4.w), D4", after 04878C "cmpi.b #-$3, $633d3.l": only FD-FF), and the FF at index 0
         ("floor_contact_masks", 4, "floor contact masks"),
         # the kicked door's four pieces, an object record each (04C52A "moveq #$3, D6", 04C532 "jsr $47c30.l", 04C538
         # "adda.w #$8, A5")
         ("kicked_door_pieces", 32, "object records"),
         # Rocksteady's six shots around, a byte each (04F9F6 "move.l #$4f9d4, ($1a,A0)", 04FA00 "move.b #$7, ($22,A0)",
         # 04FA1A "subq.b #1, ($22,A0)", 04FA2E "move.b (A4)+, D7")
         ("rocksteady_shoot_around_aims", 6, "aims"),
         # Bebop's moves by how far the turtle is: four distances and moves (0505A8 "sub.w ($0,A6), D7", 0505B0 "adda.w
         # #$4, A6", 0505B6 "cmpi.b #$4, D6", 0505C8 "move.w ($2,A6), ($28,A0)")
         ("bebop_far_moves", 16, "distances and moves, 16-bit each"),
         # Shredder's moves by how far the turtle is: four distances, then the four moves 8 bytes on (052AA6 "moveq #$3,
         # D6", 052AA8 "cmp.w (A6), D1", 052AAE "adda.w #$2, A6", 052ABA "move.w ($8,A6), ($28,A0)")
         ("shredder_distance_moves", 16, "distances and moves, 16-bit each"),
         # a Mouser's arc as he breaks down (0519E4 "lea $519d8.l, A6", then enemy_start_arc), the 12 bytes of a struct
         # arc: enemy_arc_step reads its last word too (0486BE "cmp.w ($a,A6), D0"), past what the record walk measures
         ("enemy_mouser_break_down_arc", 12, "arc"),
         # the y's that pick Stockman's height, 4 pixels apart: 15 of them, of which the loop compares only the first
         # (013FA4 "cmp.w ($0,A6), D6" without moving A6, 013FAC "addi.w #$2, D5", 013FB0 "cmpi.b #$1e, D5"), and the 16
         # heights it picks from (013FB6 "lea $14194.l, A6", 013FBC "move.w (A6,D5.w), D6")
         ("stockman_height_steps", 30, "depths, 16-bit each"), ("stockman_step_heights", 32, "heights, 16-bit each"),
         # a turtle's throws: four records of the move (a word) and the work RAM counter (a long) (01C072, 01C074; the
         # table is read at 6 bytes a throw)
         ("turtle_throw_moves", 24, "throw records"),
         # what no code reads: two lists of jobs for queue_jobs (a job's number and parameter a word each, to FFFF),
         # the building site soldier's four waits copied for the Technodrome's, a door piece's object record, the
         # Technodrome soldier's
         ("unused_asia_warning_jobs", 6, "job list"), ("unused_konami_logo_jobs", 6, "job list"),
         ("unused_rock_soldier_technodrome_waits", 4, "waits"),
         ("unused_double_door_piece_record", 8, "object records"),
         # the unused animation viewer's counts: 82 entries' animations and the FF that ends them (0084D4 "cmp.b
         # (A6,D6.w), D7", 00850C "move.b (A6,D6.w), D6", 008510 "bpl $851a"); and the old table of the attract mode's
         # steps after them, 7 routines no code reads
         ("animation_viewer_counts", 83, "animation counts"), ("unused_attract_steps", 7 * 4, "code table"),
         # the addresses of the game over's two palette effect lists after game_step_palettes, which no code reads
         ("unused_game_over_effect_lists", 8, "addresses of palette effect lists, 32-bit each"),
         # the viewer's entries, 82 of 8 bytes (00859C "lsl.w #3, D6"), and the FFFFFFFF after them: from the second
         # column on (the walk of the animations takes the first entry's table as a table of animation tables), as
         # far as an object's y of 0x8800 (01354E "move.l #$8800, ($16,A0)") would stop the walk of tables read by an
         # index
         ("animation_viewer_palettes", 82 * 8, "indexed table of 32-bit values"))
# tables of records whose count the code fixes, a span a record: (name in symbols.txt of the first, records, bytes
# each, kind)
RECORDS = (
    # the 50 records of set_object_stats, 10 bytes each from the first (hydrant_stats) to 0182ED, the ones no code
    # loads too (pizza_stats_by_kind holds four, 018136-01815D; unused_freezing_stats, unused_knock_flying_stats)
    ("hydrant_stats", 50, 10, "object stats"),)
# the tables of addresses whose entries the walk follows into data of one kind: {name in symbols.txt: (kind, the
# Walk method that measures the data at an entry)}. A table ends where the first thing it points at starts
POINTED = {
    # the knocked-down turtles' steps: bytes up to a negative one, the step to go back to (01C74C "move.b (A6,D0.w),
    # D0", 01C750 "bmi $1c75e", 01C75E "move.b D0, ($24,A0)")
    "turtle_knocked_down_steps": ("knock steps", "negative_ended"),
    "turtle_knocked_back_steps": ("knock steps", "negative_ended"),
    "turtle_knocked_flying_steps": ("knock steps", "negative_ended"),
    "turtle_knocked_flying_from_behind_steps": ("knock steps", "negative_ended"),
    # a row of freeze nozzles: how many less one, then each one's place (010A32 "move.b (A6)+, D7", 010A36 "move.b
    # (A6)+, D0", 010A88 "dbra D7, $10a34")
    "nozzle_rows": ("nozzle row", "count_prefixed"),
    # April talking on the TV: 3 rows of 3 tiles (01F0BA "moveq #$2, D7", 01F0BC "move.b (A6), (A0)" to 01F0C4
    # "move.b ($2,A6), (A1)", 01F0D0 "addq.l #3, A6")
    "street_tv_pictures": ("tv frame", "tv_frame"),
    # the burning roof's fire before area 0: two squares of 8 x 8 tiles, two of 4 x 4 (an address in the tile RAM, a
    # colour, a row's first tile each, a byte skipped: 0310F8, 031118), and one of 4 x 4 tiles one by one (031138)
    "intro_fire_frames": ("fire picture frame", "fire_picture_frame"),
}
TV_FRAME = 9
FIRE_PICTURE_FRAME = 2 * (4 + 1 + 8 + 1) + 2 * (4 + 1 + 4 + 1) + (4 + 1 + 16)
# the animations no code starts (symbols.txt names them), walked as the ones it does
UNUSED_ANIMATIONS = ("unused_april_tied_up_animation",)
# a Mouser hole's record (mouser_hole_animation_tables' entries): its closed and broken animations, then the record of
# the Mouser it lets out (051BC2 "adda.w #$8, A4", 051BFC "movea.l ($24,A0), A5", 051C00 "jsr $47c30.l")
MOUSER_HOLES, HOLE_RECORD, OBJECT_RECORD = "mouser_hole_animation_tables", 8, 8
# Rocksteady's shots: two records of an animation and the address of a shot's record, 6 bytes, for start_shot
# (04FDDC "lea $4fda8.l, A5", 04FDF2 "adda.w #$8, A5", 04FDF6 "movea.l (A5)+, A6", 04FDF8 "move.l (A5), ($24,A0)";
# 04FE14 "movea.l ($24,A0), A5", 04FE18 "jsr $47c5a.l")
SHOT_PAIRS, SHOT_PAIR_COUNT, SHOT_RECORD = "rocksteady_shoot_animations", 2, 6
# the texts print_text_at writes (positioned_texts, by D6): a 16-bit offset in the layer text_layer_places gives
# for D7, then the characters, '/' and the next line's offset, '?' the end
POSITIONED_TEXTS = "positioned_texts"
# the scroll's segments (area_scroll_segments, a list an area: the addresses of its segments, the last one's type
# 0): a segment's type (1 byte: 0 the end, negative 16 bytes, else 42), its axis and direction (bit 7: y), the
# scroll position it starts at (32 bits), then its limits and speeds
SEGMENT_LONG, SEGMENT_SHORT, SEGMENT_END = 42, 16, 2
LONG_NAMES = ("area_scroll_segments", "screen_picture_tiles", "screen_picture_colours", "demo_settings", "demo_inputs")
# the attract mode's demos (start_demo, by 0600B6): demo_settings, 4 bytes a demo (its area...); demo_inputs, the
# addresses of its 4 players' inputs; an input stream is pairs of bytes (the inputs, how many frames), as long
# as the demo lasts: up to the next stream, the last one up to the next thing
DEMOS, PLAYERS = 3, 4
# the typed texts (start_typed_text, start_typed_line: A6 the text): a line's tile RAM address (32 bits), the
# frames between its characters, a colour byte, then the characters ('/' another line, '!' the end)
TYPED_NAMES = ("start_typed_text", "start_typed_line")
TYPED_LINE, TYPED_END = 0x2F, 0x21
MAX_TYPED = 1024
# what is left: a table the code reads by an index from a fixed address (lea T, An then (d,An,Xn), or
# (d,PC,Xn)), its elements as wide as the read, up to the next thing walked, code, or place the code names
ELEMENT_SIZES = {"b": 1, "w": 2, "l": 4}
# the picture draw_screen_picture puts in layer A: 13 rows of 24 tiles' colours, and their codes
PICTURE = 13 * 24
# the palette effects (start_palette_effects takes a list in A6: a 16-bit count, then the addresses of that many
# effects): an effect is its colour (bank << 8 | palette << 4 | colour number), then steps of 4 bytes, a 16-bit
# control (its low byte the frames to show the step, bit 15 the end, bit 14 a jump to the address after it,
# bit 13 back the other way, bit 12 forwards again) and the colour; it is read up to its end, its jump or its
# turn back. The code hands lists one at a time, or tables of 0-closed lists of lists (by the scene, the area)
EFFECT_STEP = 4
EFFECT_END, EFFECT_JUMP, EFFECT_BACK = 0x8000, 0x4000, 0x2000
# the fire's tiles (animate_fire_tiles, 01FDCA): 4 phases, each an address in the tile RAM and 8 frames; a
# frame 3 rows of blocks' offsets in fire_blocks (17 bytes a block), 4 blocks a row in area 0, 8 elsewhere
FIRE_PHASES, FIRE_FRAMES, FIRE_ROWS = 4, 8, 3
# the enemies: area_spawn_lists, an address a area of its spawn list: 8 bytes an enemy (x where the screen's
# scroll brings it in, y, height, flags: bits 0-3 the players there must be at least, bit 6 the side it comes
# from; its kind), closed by an x of FFFF; spawn_offsets, 4 bytes by the kind (the low 7 bits)
SPAWN = 8
SPAWN_OFFSET = 4
# the layers' maps: area_layers (36 bytes an area, AREAS of them: two layers' starting scroll, map, blocks and
# map width), and the three sets of a layer's variables in work RAM (the layer being drawn, layer A, layer B:
# the map's address at +12, the blocks' at +16, the map's width in bytes at +10)
AREAS = 10
AREA_LAYERS_RECORD = 36
AREA_LAYERS_FIELDS = ((0x10, 0x14, 0x20), (0x18, 0x1C, 0x22))     # (map, blocks, width) of each layer
LAYER_VARIABLES = (0x063300, 0x063340, 0x063380)
MAP, BLOCKS, WIDTH = 0x12, 0x16, 0x10
NEAR = 16                       # how many instructions apart the stores of one map, its blocks and width are
DEFAULT_WIDTH = 0x20
BLOCK, FLOOR_BLOCK = 17, 33     # a block: its colour and 4 x 4 tiles; with the floor: its colour, 16 floor bytes, the tiles
MAP_ROWS = 8                    # the rows the block size is told from
# the palettes: post_palette_request takes a kind and a mask of the 4 banks in D7 (kind << 8 | mask) and data in
# A6. Kind 1 sets the area's palettes (area_palettes, by the area: a palette set each), kind 2 the palette set
# in A6, kind 4 one palette (bank and slot in D7's low byte), kind 5 single colours (a list of slot, colour
# number and colour, closed by a negative byte). A palette set is 4 addresses of palette lists, a bank each;
# a list holds a bank's count (palette_bank_counts, less one) of palette addresses; a palette is 16 colours
# of 16 bits (xBGR 555)
PALETTE = 32
BANKS = 4
ALTERNATE_BANK = 3
SEARCH_BACK = 6                 # how many instructions before a call its arguments are looked for
# the tables of animations the analysis says A6 holds an entry of, by how many tables deep the animation is
TABLE_DEPTHS = {m68000_analysis.ENTRY: 0, m68000_analysis.ENTRY2: 1, m68000_analysis.ENTRY3: 2}
TABLE_KINDS = ("animation table", "table of animation tables", "table of tables of animation tables")
# the instructions of the engines that read the data the walk covers (for the check against the reads)
ENGINE_READERS = (range(0x046F92, 0x04716E), range(0x0471AE, 0x0471F0), range(0x047204, 0x047330),
                  range(0x047534, 0x047662), range(0x047662, 0x047770), range(0x047BF2, 0x047C02),
                  range(0x01E050, 0x01E1BA), range(0x01E740, 0x01EBCE))


def table_places():
    """{name: address} of symbols.txt's table lines ('ADDRESS table NAME[COUNT] TYPE'), which the analysis does not
    read"""
    places = {}
    for line in (ROOT / "symbols.txt").read_text(encoding="utf-8").splitlines():
        parts = line.split("#", 1)[0].split()
        if len(parts) >= 3 and parts[1] == "table":
            places[parts[2].partition("[")[0]] = int(parts[0], 16)
    return places


class Walk:
    """What the walk found: spans {first: (stop, kind)}, and the problems it ran into"""

    def __init__(self, analysis):
        self.analysis = analysis
        self.mem = analysis.mem
        self.spans = {}
        self.owner = {}             # every byte walked -> where its span starts
        self.problems = []
        # what the analysis says a register may hold (its sets are of what some way can give it, the object's
        # fields what any object stores there) that turns out to be no animation: not walked, and no problem
        self.rejected = []
        self.animations = set()     # where the animations the code starts begin
        self.tables = {}            # animation tables: {address: entries}
        self.lists = {}             # sprite lists: {address: sprites}
        self.boxes = set()          # body box offsets
        self._starts = None         # the analysis's block starts, sorted (state_at)
        self.loaded = {}            # {table: the instruction that loads it} of the tables loaded by lea
        self.leftover_tables = []   # the animation tables no path the analysis follows hands on
        # the places symbols.txt names that the walk starts from or reads (its data lines, and its table lines)
        names = table_places()
        names.update({symbol.name: address for address, symbol in analysis.symbols.items()})
        wanted = NAMES + tuple(name for name, _, _ in FIXED) + TEXT_TABLES + (POSITIONED_TEXTS,) + LONG_NAMES + TYPED_NAMES \
            + tuple(POINTED) + UNUSED_ANIMATIONS + (MOUSER_HOLES, SHOT_PAIRS, "object_routine_tables") \
            + tuple(name for name, _, _, _ in RECORDS)
        missing = [name for name in wanted if name not in names]
        if missing:
            raise SystemExit("data_walk: symbols.txt names no " + ", ".join(missing))
        self.places = {name: names[name] for name in wanted}

    def in_program(self, address):
        return m68000_analysis.CODE_START <= address < m68000_analysis.CODE_END and address % 2 == 0

    def claim(self, first, stop, kind):
        """The bytes first..stop are a 'kind'; False if they are code or something else already"""
        for address in range(first, stop):
            if address in self.analysis.owner:
                self.problems.append(f"{kind} at {first:06X} runs into code at {address:06X}")
                return False
        here = self.spans.get(first)
        if here == (stop, kind):
            return True
        taken = {self.owner[address] for address in range(first, stop) if address in self.owner}
        if here is not None:
            taken.add(first)
        if taken and all(self.spans[start][1] == kind for start in taken):
            # two of a kind that share bytes (a sprite list that starts inside another, a table the code reads
            # from an entry on): one span
            first = min([first] + list(taken))
            stop = max([stop] + [self.spans[start][0] for start in taken])
            for start in taken:
                del self.spans[start]
            self.spans[first] = (stop, kind)
            for address in range(first, stop):
                self.owner[address] = first
            return True
        if taken:
            other = here if here is not None else (self.spans[min(taken)][0], self.spans[min(taken)][1])
            start = first if here is not None else min(taken)
            self.problems.append(f"{kind} at {first:06X}-{stop - 1:06X} runs over a {other[1]} at {start:06X}-{other[0] - 1:06X}")
            return False
        self.spans[first] = (stop, kind)
        for address in range(first, stop):
            self.owner[address] = first
        return True

    def free(self, first, stop):
        """No byte of first..stop walked yet, nor code"""
        return not any(address in self.owner or address in self.analysis.owner for address in range(first, stop))

    # ---- reading without walking: is it one? ----

    def frame(self, address):
        """The length of the frame at 'address' (10, or 18 with an attack box), or None if it is none"""
        if not self.in_program(address):
            return None
        sprites = self.mem.r32(address + 2)
        if sprites >> 24 or not self.in_program(sprites) or self.mem.r16(address + 6) % 4                 or self.mem.r16(address + 6) >= MAX_BOX or sprites in self.analysis.owner                 or not self.sprite(sprites):
            return None
        return FRAME if self.mem.r16(address + 8) & 0x8000 else ATTACK_FRAME

    def sprite(self, address):
        """Does the sprite at 'address' look like one: flips in its top 2 bits and nothing below them, its x
        and y within the object's reach"""
        if self.mem.r8(address + 3) & 0x3F:
            return False
        return all(-SPRITE_REACH <= value - (0x10000 if value & 0x8000 else 0) < SPRITE_REACH
                   for value in (self.mem.r16(address + 4), self.mem.r16(address + 6)))

    def frames(self, address):
        """The records of an animation from 'address', [(address, length, kind)], and the frame its end goes
        on from (None if it stops, or runs into another animation: one the code starts on its own, whose
        first frame the engine shows only from there); None when it is no animation"""
        found = []
        for _ in range(MAX_FRAMES):
            if found and address in self.animations:
                return found, None
            if not self.in_program(address):
                return None
            # (the first is a frame whatever its count: a 0 there keeps the animation on it)
            if found and self.mem.r8(address) == 0:
                if self.mem.r8(address + 1) == 0:
                    found.append((address, 2, "animation end"))
                    return found, None
                target = self.mem.r32(address + 2)
                if target >> 24 or self.frame(target) is None:
                    # a jump to no frame: an end the engine never steps on to (02DF50 "00 FF" runs into the
                    # next animation, whose first bytes would be its address)
                    found.append((address, 2, "animation end"))
                    return found, None
                found.append((address, 6, "animation jump"))
                return found, target
            length = self.frame(address)
            if length is None:
                return None
            found.append((address, length, "animation frame"))
            if len(found) == 1 and self.mem.r8(address) == 0:
                return found, None
            address += length
        return None

    # ---- the walk ----

    def holds(self, value, depth):
        """Is 'value' the address of an animation (depth 0), of a table of them (1), or of a table of those (2)"""
        if value >> 24 or not self.in_program(value):
            return False
        return self.frame(value) is not None if depth == 0 else self.holds(self.mem.r32(value), depth - 1)

    def animation_table(self, table, origin, depth=0):
        """A table the code picks an animation from (depth 0), or a table of them (1, 2): up to the first entry
        that is none, or where something else starts"""
        entries = []
        for index in range(MAX_TABLE):
            where = table + 4 * index
            value = self.mem.r32(where)
            if index and (where in self.analysis.owner or where in self.animations or where in self.tables):
                break
            if not self.holds(value, depth):
                break
            entries.append(value)
        if not entries:
            self.rejected.append(f"no {TABLE_KINDS[depth]} at {table:06X} ({origin})")
            return
        if table in self.tables:
            return
        self.tables[table] = (depth, entries)
        for value in entries:
            if depth:
                self.animation_table(value, f"the table at {table:06X}", depth - 1)
            else:
                self.animations.add(value)

    def record_animations(self, table, offset):
        """The animations at 'offset' in the records a table of addresses points at: the table up to the
        first entry that is none (the records themselves are another engine's: only the animations here)"""
        entries = []
        for index in range(MAX_FRAMES):
            where = table + 4 * index
            record = self.mem.r32(where)
            if index and (where in self.analysis.owner or where in self.tables):
                break
            if record >> 24 or not self.in_program(record) or not self.holds(self.mem.r32(record + offset), 0):
                break
            entries.append(record)
        if not entries:
            self.rejected.append(f"no records with an animation at +{offset} from {table:06X}")
            return
        self.animations.update(self.mem.r32(record + offset) for record in entries)

    # ---- the layers' maps ----

    def layer_sets(self):
        """(map, blocks, width) of every map the code gives a layer: area_layers' records, and the immediates
        it stores in a layer's variables (the title, the intermissions...)"""
        found = set()
        table = self.places["area_layers"]
        for area in range(AREAS):
            record = table + AREA_LAYERS_RECORD * area
            for map_field, blocks_field, width_field in AREA_LAYERS_FIELDS:
                found.add((self.mem.r32(record + map_field), self.mem.r32(record + blocks_field),
                           self.mem.r16(record + width_field)))
        stores = []     # (instruction number, the layer's variables, which one, the value)
        for number, address in enumerate(sorted(self.analysis.instructions)):
            instruction = self.analysis.instructions[address]
            operands = instruction.operands
            if instruction.mnemonic != "move" or len(operands) != 2 or operands[0].mode != "immediate"                     or operands[1].mode != "abs_long":
                continue
            for variables in LAYER_VARIABLES:
                if (operands[1].value & 0xFFFFFF) - variables in (MAP, BLOCKS, WIDTH):
                    stores.append((number, variables, (operands[1].value & 0xFFFFFF) - variables,
                                   operands[0].value & 0xFFFFFF))
        for number, variables, which, value in stores:
            if which != MAP:
                continue
            near = [store for store in stores if store[1] == variables and abs(store[0] - number) <= NEAR]
            blocks = [store for store in near if store[2] == BLOCKS and store[0] > number]
            widths = [store for store in near if store[2] == WIDTH]
            if not blocks:
                self.problems.append(f"the map {value:06X} has no blocks stored near it")
                continue
            width = min(widths, key=lambda store: abs(store[0] - number))[3] if widths else DEFAULT_WIDTH
            found.add((value, min(blocks)[3], width))
        return found

    def layers(self):
        """The maps (rows of 16-bit offsets of blocks, as many rows as are blocks' offsets before the next
        thing) and the blocks they use (up to the furthest one)"""
        sets = sorted(self.layer_sets())
        starts = sorted({address for layer in sets for address in layer[:2]} | set(self.spans)
                        | {self.places["fire_blocks"]})
        reach = {}
        for address, blocks, width in sets:
            following = [start for start in starts if start > address]
            limit = min(following) if following else m68000_analysis.CODE_END
            rows = []
            while address + width * (len(rows) + 1) <= limit:
                row = address + width * len(rows)
                if any(byte in self.analysis.owner for byte in range(row, row + width)):
                    break
                rows.append([self.mem.r16(row + 2 * column) for column in range(width // 2)])
            sample = [value for row in rows[:MAP_ROWS] for value in row]
            if not sample or not width:
                self.problems.append(f"no map at {address:06X}")
                continue
            # the blocks' size, as the values tell it: 33 when they are all multiples of 33 but not of 17
            size = FLOOR_BLOCK if all(value % FLOOR_BLOCK == 0 for value in sample)                 and not all(value % BLOCK == 0 for value in sample) else BLOCK
            count = 0
            while count < len(rows) and all(value % size == 0 for value in rows[count]):
                count += 1
            end = address + width * count
            values = [value for row in rows[:count] for value in row]
            # entries past the last whole row, up to the next thing: the map's tail no scroll reaches (area 5's,
            # 03372C: 8 entries after its 13 rows of 0xD0 bytes, before its blocks)
            tail = [self.mem.r16(at) for at in range(end, limit - 1, 2)]
            if count == len(rows) and 0 < limit - end < width and (limit - end) % 2 == 0 and self.free(end, limit) \
                    and all(value % size == 0 for value in tail):
                end = limit
                values += tail
            self.claim(address, end, "layer map")
            before = reach.get(blocks)
            if before is not None and before[0] != size:
                self.problems.append(f"the blocks at {blocks:06X} are of {before[0]} and {size} bytes")
            reach[blocks] = (size, max([value for value in values] + ([before[1]] if before else [])))
        for blocks, (size, top) in sorted(reach.items()):
            kind = "layer blocks" if size == BLOCK else "layer blocks with floor"
            stop = blocks + top + size
            self.claim(blocks, stop, kind)
            # the blocks after the furthest a map uses, up to the next thing (a byte of padding at most left)
            following = [start for start in starts if start >= stop]
            limit = min(following) if following else stop
            unused = (limit - stop) // size * size
            if unused and limit - stop - unused <= 1 and self.free(stop, stop + unused):
                self.claim(stop, stop + unused, kind.replace("layer blocks", "layer blocks no map uses"))
        record = AREA_LAYERS_RECORD * AREAS
        self.claim(self.places["area_layers"], self.places["area_layers"] + record, "area layers")

    # ---- the palettes ----

    def arguments(self, target, registers):
        """For every call or jump to 'target': {register: the immediate it was set to} from the instructions
        just before it (lea, movea.l #, moveq, move #), within the same run of code"""
        found = []
        ordered = sorted(self.analysis.instructions)
        for number, address in enumerate(ordered):
            instruction = self.analysis.instructions[address]
            if instruction.target != target or instruction.flow not in (m68000_analysis.dis.FLOW_CALL,
                                                                        m68000_analysis.dis.FLOW_JUMP):
                continue
            values = {}
            for back in ordered[max(0, number - SEARCH_BACK):number][::-1]:
                before = self.analysis.instructions[back]
                operands = before.operands
                if before.flow != m68000_analysis.dis.FLOW_NORMAL or back + before.length not in self.analysis.instructions:
                    break
                if len(operands) == 2 and operands[1].mode in ("areg", "dreg") and operands[1].reg in registers                         and operands[1].reg not in values:
                    source = operands[0]
                    if before.mnemonic in ("lea", "movea", "move", "moveq") and source.mode in                             ("immediate", "quick", "abs_long", "abs_short", "pc_disp") and                             (before.mnemonic == "lea" or source.mode in ("immediate", "quick")):
                        values[operands[1].reg] = source.value & 0xFFFFFF
                    else:
                        values[operands[1].reg] = None
            found.append((address, values))
        return found

    def state_at(self, address):
        """What the analysis says the address registers hold just before the instruction at 'address'"""
        if self._starts is None:
            self._starts = sorted(self.analysis.entry_states)
        start = self._starts[bisect_right(self._starts, address) - 1]
        state = dict(self.analysis.entry_states[start])
        while start < address:
            instruction = self.analysis.instructions[start]
            if instruction.flow == m68000_analysis.dis.FLOW_CALL:
                kept = self.analysis.preserved.get(instruction.target, ())
                state = {register: values for register, values in state.items() if register in kept}
            elif instruction.flow == m68000_analysis.dis.FLOW_NORMAL:
                self.analysis.step(instruction, state)
            start += instruction.length
        return state

    def held(self, address, register):
        """The values the analysis says 'register' holds just before the instruction at 'address'"""
        return m68000_analysis.known(self.state_at(address).get(register))

    def palette(self, address):
        self.claim(address, address + PALETTE, "palette")

    def is_palette_set(self, address):
        if not self.in_program(address):
            return False
        lists = [self.mem.r32(address + 4 * bank) for bank in range(BANKS)]
        return any(lists) and all(not listed or self.in_program(listed) and self.in_program(self.mem.r32(listed))
                                  for listed in lists)

    def palette_set(self, address, mask=0xF):
        """4 palette lists, a bank each (the ones the mask names are read)"""
        counts = self.places["palette_bank_counts"]
        self.claim(address, address + 4 * BANKS, "palette set")
        for bank in range(BANKS):
            listed = self.mem.r32(address + 4 * bank)
            if not mask & (1 << bank) or not listed:
                continue
            self.palette_list(listed, bank)

    def palette_list(self, address, bank):
        """The bank's count of palettes; bank 3's twice that: a second half it takes when 060BFB is set
        (0093CA sets it for bank 3, and 0093BC, 0094C0 then add 0x20 to the list)"""
        count = (self.mem.r8(self.places["palette_bank_counts"] + bank) + 1) * (2 if bank == ALTERNATE_BANK else 1)
        self.claim(address, address + 4 * count, "palette list")
        for index in range(count):
            self.palette(self.mem.r32(address + 4 * index))

    def unused_palettes(self):
        """A gap of whole palettes between two palettes: palettes no list names. A gap of less than a palette's
        colours: the colours past the palette's 16th, which a palette request never copies (the turtles' palettes
        are 18 colours apart, 00CD24-00CDB3; 00B606). A gap of whole lists between two palette lists, every entry
        a palette: a palette list no set names (00A1DE)"""
        palettes = sorted(first for first, (stop, kind) in self.spans.items() if kind == "palette")
        for before, after in zip(palettes, palettes[1:]):
            stop = self.spans[before][0]
            if stop < after and (after - stop) % PALETTE == 0 and self.free(stop, after):
                self.claim(stop, after, "palette no list names")
            elif stop < after < stop + PALETTE and (after - stop) % 2 == 0 and self.free(stop, after):
                self.claim(before, after, "palette")
        palettes = set(first for first, (stop, kind) in self.spans.items() if kind in ("palette", "palette no list names"))
        lists = sorted(first for first, (stop, kind) in self.spans.items() if kind == "palette list")
        for before, after in zip(lists, lists[1:]):
            stop = self.spans[before][0]
            if stop < after and (after - stop) % 4 == 0 and self.free(stop, after) \
                    and all(self.mem.r32(entry) in palettes for entry in range(stop, after, 4)):
                self.claim(stop, after, "palette list")

    def colours(self, address):
        """slot, colour number and colour (4 bytes) each, closed by a negative byte"""
        stop = address
        while self.mem.r8(stop) < 0x80:
            stop += 4
        self.claim(address, stop + 1, "palette colours")

    def palettes(self):
        table = self.places["area_palettes"]
        self.claim(table, table + 4 * AREAS, "area palettes")
        self.claim(self.places["palette_bank_counts"], self.places["palette_bank_counts"] + BANKS, "palette bank counts")
        for area in range(AREAS):
            self.palette_set(self.mem.r32(table + 4 * area))
        for call, values in self.arguments(self.places["copy_palette_sets"], ("A5", "D6")):
            # palette sets one after the other, the banks D6's high byte names
            table, banks = values.get("A5"), values.get("D6")
            if table is None or banks is None or not self.in_program(table):
                self.problems.append(f"the palette sets at {call:06X}: no table in A5 or no banks in D6")
                continue
            count = 0
            while self.is_palette_set(table + 16 * count) and self.free(table + 16 * count, table + 16 * count + 16):
                self.palette_set(table + 16 * count, banks >> 8)
                count += 1
            if not count:
                self.problems.append(f"the palette sets at {call:06X}: none at {table:06X}")
        for call, values in self.arguments(self.places["post_palette_request"], ("A6", "D7")):
            data, request = values.get("A6"), values.get("D7")
            if request is None:
                # (unreached_009264 posts the requests of a list in A4: the list is no code's)
                self.rejected.append(f"the palette request at {call:06X}: no kind in D7")
                continue
            kind, mask = (request >> 8) & 0x3F, request & 0xFF
            if kind == 3 and data is not None and self.in_program(data):
                # a palette list, for the first bank of the mask
                self.palette_list(data, min(bank for bank in range(BANKS) if mask & (1 << bank)))
                continue
            if kind in (0, 1, 6, 7, 8):
                continue        # (no data in A6: the area's palettes, a fade, nothing)
            if data is None and kind == 2:
                # a table of palette sets the code picks one from (009A6C "movea.l (A6,D7.w), A6")
                held = self.held(call, "A6")
                tables = [value for kind_held, value in held if kind_held == m68000_analysis.ENTRY]
                if not tables and any(kind_held == m68000_analysis.FIELD for kind_held, _ in held):
                    # a field of a table of records: the unused animation viewer's palette sets (0085AE "movea.l
                    # ($4,A5,D6.w), A6"), which FIXED walks with the records; their sets are other requests' too
                    self.rejected.append(f"the palette request at {call:06X}: palette sets in a table of records, "
                                         "walked with it")
                    continue
                for table in tables:
                    count = 0
                    while self.is_palette_set(self.mem.r32(table + 4 * count)) and table + 4 * count not in self.spans:
                        self.palette_set(self.mem.r32(table + 4 * count), mask)
                        count += 1
                    self.claim(table, table + 4 * count, "table of palette sets")
                if not tables:
                    self.problems.append(f"the palette request at {call:06X}: kind {kind} with no data in A6")
            elif data is None or not self.in_program(data):
                self.problems.append(f"the palette request at {call:06X}: kind {kind} with no data in A6")
            elif kind == 2:
                self.palette_set(data, mask)
            elif kind == 4:
                self.palette(data)
            elif kind == 5:
                self.colours(data)
            else:
                self.problems.append(f"the palette request at {call:06X}: kind {kind}")

    # ---- the enemies ----

    def spawns(self):
        table = self.places["area_spawn_lists"]
        self.claim(table, table + 4 * AREAS, "area spawn lists")
        kinds = set()
        for area in range(AREAS):
            spawn = self.mem.r32(table + 4 * area)
            start = spawn
            while self.mem.r16(spawn) != 0xFFFF:
                kinds.add(self.mem.r8(spawn + 7) & 0x7F)
                spawn += SPAWN
            self.claim(start, spawn + 2, "spawn list")
        for name in ("area_spawn_scrolls", "area_spawn_starts"):
            self.claim(self.places[name], self.places[name] + 8 * AREAS, name.replace("_", " "))
        # an offset for every kind of object, not only the ones the spawn lists start: update_object_screen_place
        # reads it by any object's kind (0478DA "lea $53a5e.l, A4", 0478E0 "move.b ($2,A0), D4"); the kinds are
        # object_routine_tables' entries, which run_objects reads by the kind (047812 "lea $53896.l, A6") and which
        # end where spawn_offsets starts: 0x72 of them
        offsets = self.places["spawn_offsets"]
        count = (offsets - self.places["object_routine_tables"]) // 4
        if max(kinds) >= count:
            self.problems.append(f"a spawn list starts kind {max(kinds):02X}, past the {count:02X} kinds")
        self.claim(offsets, offsets + SPAWN_OFFSET * count, "spawn offsets")

    # ---- the fire's tiles ----

    def fire(self):
        top = 0
        for name, columns in (("fire_tiles_area_0", 4), ("fire_tiles", 8)):
            table = self.places[name]
            self.claim(table, table + 4 * FIRE_PHASES, "fire phases")
            for phase in range(FIRE_PHASES):
                record = self.mem.r32(table + 4 * phase)
                self.claim(record, record + 4 + 4 * FIRE_FRAMES, "fire phase")
                for frame in range(FIRE_FRAMES):
                    rows = self.mem.r32(record + 4 + 4 * frame)
                    length = 2 * columns * FIRE_ROWS
                    self.claim(rows, rows + length, "fire frame")
                    top = max([top] + [self.mem.r16(rows + at) for at in range(0, length, 2)])
        self.claim(self.places["fire_blocks"], self.places["fire_blocks"] + top + BLOCK, "layer blocks")

    # ---- the palette effects ----

    def effect(self, address):
        step = address + 2
        while True:
            control = self.mem.r16(step)
            if control & EFFECT_JUMP and not control & EFFECT_END:
                self.claim(address, step + 6, "palette effect")
                return
            if control & EFFECT_END:
                self.claim(address, step + EFFECT_STEP, "palette effect")
                return
            if control & EFFECT_BACK and not self.free(step + EFFECT_STEP, step + 2 * EFFECT_STEP):
                # (a turn back with something else after it: the effect goes back and forth from here)
                self.claim(address, step + EFFECT_STEP, "palette effect")
                return
            step += EFFECT_STEP

    def effect_length(self, address, stop):
        """The bytes of the effect at 'address' if it ends by 'stop' (an end, a jump, or a turn back that the
        next effect or 'stop' follows), else None"""
        step = address + 2
        while step + EFFECT_STEP <= stop:
            control = self.mem.r16(step)
            if control & EFFECT_JUMP and not control & EFFECT_END:
                return step + 6 - address if step + 6 <= stop else None
            if control & EFFECT_END:
                return step + EFFECT_STEP - address
            if control & EFFECT_BACK and (step + EFFECT_STEP == stop or self.effect_length(step + EFFECT_STEP, stop)):
                return step + EFFECT_STEP - address
            step += EFFECT_STEP
        return None

    def unused_effects(self):
        """What follows a palette effect up to the next thing, when it is all effects: effects no list names"""
        for first, (stop, kind) in sorted(self.spans.items()):
            if kind != "palette effect":
                continue
            end = stop
            while end < m68000_analysis.CODE_END and self.free(end, end + 1):
                end += 1
            effects, at = [], stop
            while at < end:
                length = self.effect_length(at, end)
                if not length:
                    break
                effects.append((at, at + length))
                at += length
            if at == end and effects:
                for start, finish in effects:
                    self.claim(start, finish, "palette effect no list names")

    def effect_list(self, address):
        count = self.mem.r16(address)
        self.claim(address, address + 2 + 4 * count, "palette effect list")
        for index in range(count):
            self.effect(self.mem.r32(address + 2 + 4 * index))

    def effects(self):
        for kind, value in m68000_analysis.known(self.analysis.entry_states.get(
                self.places["start_palette_effects"], {}).get("A6")):
            if kind == m68000_analysis.CONSTANT:
                self.effect_list(value)
            elif kind == m68000_analysis.ENTRY and self.in_program(value):
                # a 0-closed list of effect lists (030ED0 "lea $9c08.l, A0" then 009B12)
                stop = value
                while self.in_program(self.mem.r32(stop)):
                    self.effect_list(self.mem.r32(stop))
                    stop += 4
                if self.mem.r32(stop) == 0:
                    self.claim(value, stop + 4, "palette effect lists")
            elif kind == m68000_analysis.ENTRY2:
                # a table of addresses of 0-closed lists of effect lists
                count = 0
                while self.in_program(self.mem.r32(value + 4 * count)) and value + 4 * count not in self.spans:
                    lists = self.mem.r32(value + 4 * count)
                    stop = lists
                    while self.mem.r32(stop):
                        self.effect_list(self.mem.r32(stop))
                        stop += 4
                    self.claim(lists, stop + 4, "palette effect lists")
                    count += 1
                self.claim(value, value + 4 * count, "table of palette effect lists")
            else:
                self.rejected.append(f"start_palette_effects' A6 holds a {kind} of {value}: not walked")

    # ---- what a register hands a function: one thing, a table of them, a table of tables... ----

    def follow(self, values, is_one, take, kind, origin):
        """Every thing of a kind the analysis's values for a register name: an address (one), an entry of a
        table (a table of them, up to the first entry that is none), an entry of an entry (a table of such
        tables), a field of what an entry points at (a table of records' addresses, each holding one there)"""
        def table(address, is_entry, take_entry, name):
            count = 0
            while count < MAX_FRAMES and address + 4 * count not in self.analysis.owner                     and (not count or address + 4 * count not in self.spans) and is_entry(self.mem.r32(address + 4 * count)):
                count += 1
            if not count:
                self.rejected.append(f"no {name} at {address:06X} ({origin})")
                return
            self.claim(address, address + 4 * count, name)
            for index in range(count):
                take_entry(self.mem.r32(address + 4 * index))

        for held, value in sorted(m68000_analysis.known(values), key=str):
            if held == m68000_analysis.CONSTANT:
                if is_one(value):
                    take(value)
                else:
                    self.rejected.append(f"no {kind} at {value:06X} ({origin})")
            elif held == m68000_analysis.ENTRY:
                table(value, is_one, take, f"{kind} table")
            elif held == m68000_analysis.ENTRY2:
                table(value, lambda entry: self.in_program(entry) and is_one(self.mem.r32(entry)),
                      lambda entry: table(entry, is_one, take, f"{kind} table"), f"table of {kind} tables")
            elif held == m68000_analysis.FIELD2:
                base, offset = value
                count = 0
                while count < MAX_FRAMES and base + 4 * count not in self.analysis.owner                         and self.in_program(self.mem.r32(base + 4 * count))                         and is_one(self.mem.r32(self.mem.r32(base + 4 * count) + offset)):
                    take(self.mem.r32(self.mem.r32(base + 4 * count) + offset))
                    count += 1
                if not count:
                    self.rejected.append(f"no records with a {kind} at +{offset} from {base:06X} ({origin})")
            elif held == m68000_analysis.FIELD and value[1] % 4 == 0:
                base, offset = value
                table(base if is_one(self.mem.r32(base)) else base + offset, is_one, take, f"{kind} table")
            else:
                self.rejected.append(f"{origin} holds a {held} of {value}: not walked")

    # ---- the tile blocks ----

    def tile_block_size(self, address):
        if not self.in_program(address):
            return None
        width, height = self.mem.r8(address) + 1, self.mem.r8(address + 1) + 1
        if width > MAX_TILE_BLOCK[0] or height > MAX_TILE_BLOCK[1]:
            return None
        size = 2 + 2 * width * height
        return None if any(byte in self.analysis.owner for byte in range(address, address + size)) else size

    def tile_blocks(self):
        function = self.places["draw_tile_block"]
        self.follow(self.analysis.entry_states.get(function, {}).get("A6"),
                    lambda address: self.tile_block_size(address) is not None
                    and self.free(address, address + self.tile_block_size(address)),
                    lambda address: self.claim(address, address + self.tile_block_size(address), "tile block"),
                    "tile block", "draw_tile_block's A6")
        # the blocks the tables of objects' tile blocks hold (their data= fields in symbols.txt)
        for name in TILE_BLOCK_TABLES:
            table = self.places[name]
            layout = self.analysis.symbols[table].layout
            for field, block in sorted(self.analysis.data_fields.items()):
                if not table <= field < table + layout.record * layout.count:
                    continue
                if self.tile_block_size(block) is None:
                    self.problems.append(f"{name}: no tile block at {block:06X}")
                else:
                    self.claim(block, block + self.tile_block_size(block), "tile block")
        places = [values.get("D7") for _, values in self.arguments(function, ("D7",))]
        places = [place for place in places if place is not None]
        table = self.places["tile_block_places"]
        if places:
            self.claim(table, table + 4 * (max(places) + 1), "tile block places")

    # ---- the texts ----

    def text(self, address, place=4):
        at = address
        while True:
            at += place
            while self.mem.r8(at) not in (TEXT_END, TEXT_LINE):
                at += 1
            if self.mem.r8(at) == TEXT_END:
                self.claim(address, at + 1, "text")
                return
            at += 1

    def texts(self):
        for name in TEXT_TABLES:
            table = self.places[name]
            first = min(self.mem.r32(table + 4 * index) for index in range(1 + (self.mem.r32(table) - table) // 4 - 1))
            count = (first - table) // 4
            self.claim(table, table + 4 * count, "text table")
            for index in range(count):
                self.text(self.mem.r32(table + 4 * index))
        table = self.places[POSITIONED_TEXTS]
        count = (self.mem.r32(table) - table) // 4
        self.claim(table, table + 4 * count, "text table")
        for index in range(count):
            self.text(self.mem.r32(table + 4 * index), 2)

    # ---- the scroll's segments ----

    def segments(self):
        table = self.places["area_scroll_segments"]
        self.claim(table, table + 4 * AREAS, "area scroll segments")
        for area in range(AREAS):
            listed = self.mem.r32(table + 4 * area)
            count = 0
            while True:
                segment = self.mem.r32(listed + 4 * count)
                count += 1
                kind = self.mem.r8(segment)
                size = SEGMENT_END if kind == 0 else SEGMENT_SHORT if kind & 0x80 else SEGMENT_LONG
                self.claim(segment, segment + size, "scroll segment")
                if kind == 0:
                    break
            self.claim(listed, listed + 4 * count, "scroll segment list")
        for name in ("screen_picture_tiles", "screen_picture_colours"):
            self.claim(self.places[name], self.places[name] + PICTURE, name.replace("_", " "))

    # ---- the demos ----

    def demos(self):
        settings, inputs = self.places["demo_settings"], self.places["demo_inputs"]
        self.claim(settings, settings + 4 * DEMOS, "demo settings")
        self.claim(inputs, inputs + 4 * PLAYERS * DEMOS, "demo inputs")
        streams = sorted({self.mem.r32(inputs + 4 * index) for index in range(PLAYERS * DEMOS)})
        for number, stream in enumerate(streams):
            stop = streams[number + 1] if number + 1 < len(streams) else stream
            if number + 1 == len(streams):
                while stop not in self.owner and stop not in self.analysis.owner:
                    stop += 2
            self.claim(stream, stop, "demo input stream")

    # ---- the typed texts ----

    def typed_size(self, address):
        if not self.in_program(address):
            return None
        at = address
        while at - address < MAX_TYPED:
            if self.mem.r32(at) >> 20 != 0x001:     # a line starts in the K052109 (10xxxx)
                return None
            at += 6
            while at - address < MAX_TYPED and self.mem.r8(at) not in (TYPED_LINE, TYPED_END):
                at += 1
            if self.mem.r8(at) == TYPED_END:
                return at + 1 - address
            at += 1
        return None

    def typed_texts(self):
        for name in TYPED_NAMES:
            self.follow(self.analysis.entry_states.get(self.places[name], {}).get("A6"),
                        lambda address: self.typed_size(address) is not None,
                        lambda address: self.claim(address, address + self.typed_size(address), "typed text"),
                        "typed text", f"{name}'s A6")

    # ---- the tables whose entries point at data of a kind (POINTED) ----

    def negative_ended(self, address):
        """Bytes up to the first negative one, which it ends with"""
        stop = address
        while self.mem.r8(stop) < 0x80:
            stop += 1
        return stop + 1 - address

    def count_prefixed(self, address):
        """A byte, how many values less one, then the values, a byte each"""
        return 2 + self.mem.r8(address)

    def tv_frame(self, address):
        return TV_FRAME

    def fire_picture_frame(self, address):
        return FIRE_PICTURE_FRAME

    def pointed_tables(self):
        for name, (kind, measure) in sorted(POINTED.items()):
            table = self.places[name]
            entries = []
            while not entries or table + 4 * len(entries) < min(entries):
                value = self.mem.r32(table + 4 * len(entries))
                if value >> 24 or not m68000_analysis.CODE_START <= value < m68000_analysis.CODE_END:
                    break
                entries.append(value)
            if not entries:
                self.problems.append(f"{name}: no entries")
                continue
            self.claim(table, table + 4 * len(entries), "indexed table of 32-bit values")
            for value in sorted(set(entries)):
                self.claim(value, value + getattr(self, measure)(value), kind)

    # ---- records one record holds or hands on ----

    def mouser_holes(self):
        """The Mouser holes' records (MOUSER_HOLES' entries): the walk has their two animations; the object record of
        the Mouser each lets out follows them"""
        table = self.places[MOUSER_HOLES]
        stop = self.spans.get(table, (table, None))[0]
        for entry in range(table, stop, 4):
            record = self.mem.r32(entry) + HOLE_RECORD
            self.claim(record, record + OBJECT_RECORD, "object records")

    def shot_pairs(self):
        """Rocksteady's shots (SHOT_PAIRS): after each animation, the address of a shot's record, and that record"""
        table = self.places[SHOT_PAIRS]
        for index in range(SHOT_PAIR_COUNT):
            field = table + 8 * index + 4
            self.claim(field, field + 4, "addresses of shot records, 32-bit each")
            record = self.mem.r32(field)
            self.claim(record, record + SHOT_RECORD, "shot record")

    # ---- the places the code reads at their own address ----

    def absolute_reads(self):
        """A place in the program the code reads at its address (abs.l, abs.w or (d,PC)), as wide as the read: the
        tile RAM's addresses the test mode's pages start at (00584C, 0059C6), BEST 10 PLAYERS' (00917E), the box
        opening's (01867E). A lea only takes the address"""
        for address, instruction in sorted(self.analysis.instructions.items()):
            if instruction.flow != m68000_analysis.dis.FLOW_NORMAL or instruction.size not in ELEMENT_SIZES \
                    or instruction.mnemonic in ("lea", "pea"):
                continue
            operands = instruction.operands
            for index, operand in enumerate(operands):
                if operand.mode not in ("abs_long", "abs_short", "pc_disp"):
                    continue
                # (the place a move writes is no read)
                if instruction.mnemonic in ("move", "movea") and index == len(operands) - 1:
                    continue
                place = operand.value & 0xFFFFFF
                size = ELEMENT_SIZES[instruction.size]
                if m68000_analysis.CODE_START <= place < m68000_analysis.CODE_END and self.free(place, place + size):
                    self.claim(place, place + size, f"value read at its address, {8 * size}-bit")

    # ---- the tables the code reads by an index ----

    def indexed_tables(self):
        named = set()
        for instruction in self.analysis.instructions.values():
            named.update(game_profile.rom.canonical_address(value & 0xFFFFFF) or 0
                         for value in m68000_analysis.data_places(instruction))
        found = {}
        for address, instruction in sorted(self.analysis.instructions.items()):
            if instruction.flow != m68000_analysis.dis.FLOW_NORMAL or instruction.size not in ELEMENT_SIZES:
                continue
            for operand in instruction.operands:
                bases = []
                if operand.mode == "pc_index":
                    bases = [(operand.base + operand.disp) & 0xFFFFFF]
                elif operand.mode == "index":
                    bases = [(value + operand.disp) & 0xFFFFFF for held, value in self.held(address, operand.reg)
                             if held == m68000_analysis.CONSTANT]
                for base in bases:
                    if self.in_program(base) or m68000_analysis.CODE_START <= base < m68000_analysis.CODE_END:
                        size = ELEMENT_SIZES[instruction.size]
                        found[base] = max(found.get(base, 0), size)
        starts = sorted(set(found) | named)
        for base, size in sorted(found.items()):
            if base in self.owner or base in self.analysis.owner:
                continue
            following = starts[bisect_right(starts, base):]
            stop = base
            while stop < m68000_analysis.CODE_END and stop not in self.owner and stop not in self.analysis.owner                     and (stop == base or stop not in found and stop not in named):
                stop += 1
            length = (stop - base) // size * size
            if length:
                self.claim(base, base + length, f"indexed table of {8 * size}-bit values")

    # ---- the records the code reads through a register that holds their address ----

    def records(self):
        """A record the code hands a function (or loads) in a register and reads fields of, (d,An) or (An):
        from its address to the end of the furthest field read, each field as wide as its read"""
        fields = defaultdict(dict)
        for address, instruction in sorted(self.analysis.instructions.items()):
            if instruction.flow != m68000_analysis.dis.FLOW_NORMAL or instruction.size not in ELEMENT_SIZES                     or instruction.mnemonic == "lea":
                continue
            for operand in instruction.operands:
                if operand.mode not in ("disp", "indirect"):
                    continue
                offset = operand.disp if operand.mode == "disp" else 0
                for held, value in self.held(address, operand.reg):
                    if held == m68000_analysis.CONSTANT and m68000_analysis.CODE_START <= value < m68000_analysis.CODE_END:
                        fields[value][offset] = max(fields[value].get(offset, 0), ELEMENT_SIZES[instruction.size])
        for base, read in sorted(fields.items()):
            if min(read) < 0:
                continue
            stop = base + max(offset + size for offset, size in read.items())
            if self.free(base, stop):
                self.claim(base, stop, "record")

    # ---- the tables the code loads with lea and reads, whose shape no engine above gives ----

    def loaded_tables(self):
        """A place a lea or a movea.l # names in the program (not code, not walked), that the function reads
        through that register: from there to the next thing walked, code, or place the code names. Its
        shape is its reader's to tell (the decompiler's pointer types); the walk only says whose it is"""
        named = set()
        for instruction in self.analysis.instructions.values():
            named.update(game_profile.rom.canonical_address(value & 0xFFFFFF) or 0
                         for value in m68000_analysis.data_places(instruction))
        loads = {}
        for address, instruction in sorted(self.analysis.instructions.items()):
            operands = instruction.operands
            if instruction.mnemonic not in ("lea", "movea") or len(operands) != 2 or operands[1].mode != "areg":
                continue
            source = operands[0]
            if instruction.mnemonic == "lea" and source.mode in ("abs_long", "abs_short", "pc_disp"):
                place = source.value & 0xFFFFFF
            elif instruction.mnemonic == "movea" and source.mode == "immediate" and instruction.size == "l":
                place = source.value & 0xFFFFFF
            else:
                continue
            if m68000_analysis.CODE_START <= place < m68000_analysis.CODE_END:
                loads.setdefault(place, address)
        starts = sorted(set(loads) | named)
        for place, loader in sorted(loads.items()):
            if place in self.owner or place in self.analysis.owner:
                continue
            stop = place + 1
            while stop < m68000_analysis.CODE_END and stop not in self.owner and stop not in self.analysis.owner                     and stop not in named and stop not in loads:
                stop += 1
            self.claim(place, stop, "table loaded by lea")
            self.loaded[place] = loader

    def leftover_animation_tables(self):
        """In what the walk has not reached: 2 or more longs in a row (from an even address) that are each an
        animation (a whole one, the stricter frame test) and are held somewhere or named: tables of
        animations the objects' records point at (their pairs, a side each) that no path the analysis
        follows hands set_animation"""
        def animations_at(table, inside):
            count = 0
            while self.free(table + 4 * count, table + 4 * count + 4) and table + 4 * count not in inside \
                    and not self.mem.r32(table + 4 * count) >> 24 and self.frames(self.mem.r32(table + 4 * count)) is not None:
                count += 1
            return count

        def scan(inside):
            address = m68000_analysis.CODE_START
            found = []
            while address < m68000_analysis.CODE_END - 8:
                count = animations_at(address, inside)
                # (or where the one entry is the animation right after it: 0149D6, which nothing holds)
                if count >= 2 or (count == 1 and (address in held or self.mem.r32(address) == address + 4)):
                    found.append((address, count))
                    address += 4 * count
                else:
                    address += 2
            return found

        def read_by(found):
            candidates = self.animations | {self.mem.r32(table + 4 * index) for table, count in found for index in range(count)}
            inside = set()
            for animation in candidates:
                read = self.frames(animation)
                for first, length, _ in (read[0] if read else ()):
                    inside.update(range(first, first + length))
            return inside

        # (one entry is enough where some data holds the table's address: 0087CC, a palette set and the table
        # of an object's animations, 015F06)
        held = set()
        for place in range(m68000_analysis.CODE_START, m68000_analysis.CODE_END - 4, 2):
            value = self.mem.r32(place)
            if place not in self.analysis.owner and not value >> 24 and self.in_program(value):
                held.add(value)
        # a run of such longs inside what these animations read is no table (a jump's address, 014A22): the
        # second scan neither starts nor goes on there, so a table right after one is not lost to it (015F38,
        # after 015F0A's jump to itself)
        found = scan(set())
        inside = read_by(found)
        found = scan(inside)
        inside = read_by(found)
        found = [(table, count) for table, count in found if not inside & set(range(table, table + 4 * count))]
        for table, count in found:
            entries = [self.mem.r32(table + 4 * index) for index in range(count)]
            self.tables[table] = (0, entries)
            self.leftover_tables.append(table)
            self.animations.update(entries)
        return found

    def loose_ends(self):
        """A 00 00 right after an animation's frame, nothing's yet: that animation's end (the walk stops an
        animation where another one starts, and one starts right after it: 020906, 02322E...)"""
        for first, (stop, kind) in sorted(self.spans.items()):
            if kind == "animation frame" and self.free(stop, stop + 2) and self.mem.r16(stop) == 0:
                self.claim(stop, stop + 2, "animation end")
        # and a second end right after an end (02DF52: 00 00 after the 00 FF of intro_turtle_lands_animation, which
        # no frame reaches either)
        for first, (stop, kind) in sorted(self.spans.items()):
            if kind == "animation end" and self.mem.r8(first + 1) and self.free(stop, stop + 2) and self.mem.r16(stop) == 0:
                self.claim(stop, stop + 2, "animation end")

    def unreached_frames(self):
        """A run nothing has yet that is whole frames, one after another: frames no animation steps on (after
        an animation's jump, 04D39E; a frame whose 00 the engine would take for a jump, 02DB14; a still with the
        first picture of the animation after it, after a table of routines the analysis reads, 04C7A8)"""
        tables = {address for first, stop in self.analysis.described for address in range(first, stop)}

        def open_at(address):
            return self.free(address, address + 1) and address not in tables

        address = m68000_analysis.CODE_START
        while address < m68000_analysis.CODE_END:
            if not open_at(address) or not self.in_program(address):
                address += 1
                continue
            end = address
            while end < m68000_analysis.CODE_END and open_at(end):
                end += 1
            frames, at = [], address
            while at < end and self.frame(at) and at + self.frame(at) <= end:
                frames.append((at, at + self.frame(at)))
                at += self.frame(at)
            if frames and at == end:
                for first, stop in frames:
                    self.claim(first, stop, "animation frame no animation reaches")
            address = end

    def trailing_sprites(self):
        """What follows a sprite list up to the next thing, when it is all sprites: entries past the longest
        frame that shows the list (sprites no frame shows)"""
        found = 0
        for first, (stop, kind) in sorted(self.spans.items()):
            if kind != "sprite list":
                continue
            end = stop
            while end < m68000_analysis.CODE_END and self.free(end, end + 1):
                end += 1
            if end == stop or (end - stop) % SPRITE or not all(self.sprite(at) for at in range(stop, end, SPRITE)):
                continue
            if self.claim(stop, end, "sprites no frame shows"):
                found += 1
        return found

    def walk_animations(self):
            # every animation the code starts is known before one is read: one that runs into the next stops
            # there. The frames an end goes on from start animations too: read again until none is new
            records = {}
            while True:
                targets = set()
                for address in sorted(self.animations):
                    read = self.frames(address)
                    if read is None:
                        self.rejected.append(f"no animation at {address:06X}")
                        continue
                    found, target = read
                    for first, length, kind in found:
                        records[first] = (length, kind)
                    if target is not None and target not in self.animations:
                        targets.add(target)
                if not targets:
                    break
                self.animations |= targets
                records.clear()
            for table, (depth, entries) in sorted(self.tables.items()):
                self.claim(table, table + 4 * len(entries), TABLE_KINDS[depth])
            for first, (length, kind) in sorted(records.items()):
                self.claim(first, first + length, kind)
                if kind == "animation frame":
                    sprites = self.mem.r32(first + 2)
                    self.lists[sprites] = max(self.lists.get(sprites, 0), self.mem.r8(first + 1) + 1)
                    self.boxes.add(self.mem.r16(first + 6))
            # the sprite lists after the animations: a list is as long as the longest frame that shows it
            for address, count in sorted(self.lists.items()):
                self.claim(address, address + SPRITE * count, "sprite list")

    def run(self):
        held = self.analysis.entry_states.get(self.places["set_animation"], {}).get("A6")
        if held is None or held == m68000_analysis.TOO_MANY:
            raise SystemExit("data_walk: the analysis does not follow what set_animation takes in A6")
        for kind, value in sorted(held, key=str):
            if kind == m68000_analysis.CONSTANT:
                self.animations.add(value)
            elif kind in TABLE_DEPTHS:
                self.animation_table(value, "set_animation's A6", TABLE_DEPTHS[kind])
            elif kind == m68000_analysis.FIELD and value[1] % 4 == 0:
                # a table of animations the code reads from an entry on (013B1C: lea, then (4,A6,Dn)): from its
                # first entry if that is one
                table, offset = value
                self.animation_table(table if self.holds(self.mem.r32(table), 0) else table + offset,
                                     "set_animation's A6")
            elif kind == m68000_analysis.FIELD2:
                # a table of records' addresses, each with an animation at an offset (051B98: the record an
                # entry of 051AE2 points at, its first long)
                self.record_animations(*value)
            else:
                self.rejected.append(f"set_animation's A6 holds a {kind} of {value:06X}: not walked"
                                     if isinstance(value, int) else f"set_animation's A6 holds a {kind} of {value}: not walked")
        # a list of animations no code reads (unused_animation_list): every entry one
        self.animation_table(self.places["unused_animation_list"], "unused_animation_list")
        # and the animations no code starts that symbols.txt names
        for name in UNUSED_ANIMATIONS:
            self.animations.add(self.places[name])
        # the data= fields of the tables symbols.txt describes that hold animations (030DFC: records of an
        # animation and a word the code walks with (A5)+)
        for field, value in self.analysis.data_fields.items():
            if self.frame(value) is not None:
                self.animations.add(value)
        self.walk_animations()
        # the single sprites the objects keep at +3A (a shadow: draw_sprites adds it when +1 has bit 7)
        stored = set(self.analysis.stores.get(SHADOW_FIELD, ()))
        stored |= {value for kind, value in self.analysis.object_fields.get(SHADOW_FIELD, ())
                   if kind == m68000_analysis.CONSTANT and self.in_program(value)}
        for address in sorted(stored):
            if address not in self.lists:
                self.claim(address, address + SPRITE, "shadow sprite")
        self.claim(self.places["sprite_sizes"], self.places["sprite_sizes"] + SPRITE * 8, "sprite sizes")
        if self.boxes:
            # (up to the next thing, the zero boxes after the last one a frame uses: 05384E-053895)
            boxes = self.places["body_boxes"]
            stop = boxes + max(self.boxes) + 4
            while self.free(stop, stop + 4) and self.mem.r32(stop) == 0:
                stop += 4
            self.claim(boxes, stop, "body boxes")
        self.layers()
        self.palettes()
        self.effects()
        self.unused_palettes()
        self.spawns()
        self.fire()
        self.tile_blocks()
        self.texts()
        self.segments()
        self.demos()
        self.typed_texts()
        for name, size, kind in FIXED:
            self.claim(self.places[name], self.places[name] + size, kind)
        for name, count, size, kind in RECORDS:
            for record in range(self.places[name], self.places[name] + count * size, size):
                self.claim(record, record + size, kind)
        self.pointed_tables()
        self.mouser_holes()
        self.shot_pairs()
        # last: what the engines above leave
        if self.leftover_animation_tables():
            self.walk_animations()
        self.records()
        self.indexed_tables()
        self.loaded_tables()
        self.absolute_reads()
        self.trailing_sprites()
        self.unused_effects()
        self.loose_ends()
        self.unreached_frames()


def spans(analysis):
    """For the coverage (the profile's DATA_WALK): {first: (stop, kind)} of what the walk reads"""
    walk = Walk(analysis)
    walk.run()
    return walk.spans


def load_reads(folder):
    """{pc: bytes it read} from every rom_reads.txt under 'folder'"""
    reads = defaultdict(set)
    for path in sorted(Path(folder).rglob("rom_reads.txt")):
        for line in path.read_text(encoding="ascii").splitlines():
            if not line.strip() or line.startswith("#"):
                continue
            pc, span = line.split()
            first, last = (int(value, 16) for value in span.split("-"))
            reads[int(pc, 16)].update(range(first, last + 1))
    return reads


def report(walk, reads=None):
    kinds = Counter()
    for first, (stop, kind) in walk.spans.items():
        kinds[kind] += stop - first
    lines = ["Data walk", "=========", "",
             f"animations: {len(walk.animations)} (tables of them: {len(walk.tables)}, "
             f"{sum(len(entries) for _, entries in walk.tables.values())} entries), sprite lists: {len(walk.lists)}, "
             f"body boxes used: {len(walk.boxes)}; animation tables no path the analysis follows hands on (only data "
             f"points at them): {len(walk.leftover_tables)}"]
    lines += [f"  {kind:<16} {count:7d} bytes" for kind, count in sorted(kinds.items())]
    lines.append(f"  {'total':<16} {sum(kinds.values()):7d} bytes")
    if reads is not None:
        walked = {address for first, (stop, _) in walk.spans.items() for address in range(first, stop)}
        missed = defaultdict(set)
        for pc, read in reads.items():
            if any(pc in readers for readers in ENGINE_READERS):
                for address in read - walked:
                    missed[pc].add(address)
        total = set().union(*missed.values()) if missed else set()
        lines += ["", f"bytes the engines read in the scenarios that the walk did not reach: {len(total)}"]
        for pc, addresses in sorted(missed.items()):
            ordered = sorted(addresses)
            lines.append(f"  {pc:06X}  {len(ordered):6d}  {ordered[0]:06X}-{ordered[-1]:06X}")
    lines += ["", f"problems: {len(walk.problems)}"] + [f"  {problem}" for problem in walk.problems]
    lines += ["", f"values the analysis says may be animations that are none: {len(walk.rejected)}"]
    lines += [f"  {value}" for value in walk.rejected]
    return lines


def main():
    arguments = sys.argv[1:]
    reads_folder = None
    if "--reads" in arguments:
        position = arguments.index("--reads")
        if position + 1 >= len(arguments):
            raise SystemExit("Usage: python tools/data_walk.py [rom folder or .zip] [--reads FOLDER]")
        reads_folder = arguments[position + 1]
        del arguments[position:position + 2]
    source = Path(arguments[0]) if arguments else ROOT / "rom"
    image = game_profile.rom.build_program(game_profile.rom.load_set(source))
    analysis = m68000_analysis.Analysis(image, m68000_analysis.load_symbols(ROOT / "symbols.txt"))
    analysis.run()
    walk = Walk(analysis)
    walk.run()
    lines = report(walk, load_reads(reads_folder) if reads_folder else None)
    out = ROOT / "build" / "listing"
    out.mkdir(parents=True, exist_ok=True)
    (out / "data_walk.txt").write_text("\n".join(lines) + "\n", encoding="ascii")
    print("\n".join(lines[:40]))


if __name__ == "__main__":
    main()
