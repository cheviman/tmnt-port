"""
Name:        extract_layout.py
Description: writes runtime/src/extract_layout.c, what the game's first run (runtime/src/extract.c) needs to write
             the assets tools/extract_assets.py writes, without a byte of the ROM
Version:     0.1.0
Created:     04-11-2026
Modified:    04-11-2026
Author:      Sebastian Toyos

Usage: py tools/extract_layout.py [rom folder or .zip]   (rom/ by default)

The first run reads the player's ROM set in C and writes the assets folder byte for byte as extract_assets.py does.
What the Python tools work out from the static analysis and the walks it takes from this file: positions, names,
codes and formats, never a byte of the ROM (tools/rom_in_executables.py checks the executables):
  - the sheets of tiles and sprites (extract_assets.py): each sheet's name, its cells' codes in order with the palette
    each shows with (one of the sheet's 16) and whether no data or scenario took it (a neighbour), and for each of
    its 16 palettes the place of the program's data whose 16 colours it shows with (palette_ram, by the sheet's
    subject);
  - the 68000 program's data (data_assets.py): every piece's name, its file and its spans, each with the format its
    kind is written in; whether the names of addresses take it (the code read as data does not); the program's
    functions the data can name;
  - the sound program's data (sound_data_assets.py): every piece's name, kind and place, the music's too, and its
    functions;
  - the songs' files (sound_music.py): the file and the use of each command the songs table plays, and the
    velocities of the YM2151's attenuations as Python's floating point gives them.
The ROM set is the player's, read to work these out; the file holds what the static analysis and the walks found
in it. When symbols.txt, sound_symbols.txt, graphics_subjects.txt or the extractor change, run this again (the
assets check, tmnt_assets_check, compares the first run's folder with extract_assets.py's). The file is written
only when its tables change.
"""
import hashlib
import sys
from datetime import date
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path[:0] = [str(TOOLS / "core"), str(TOOLS)]
import extract_assets as ea  # noqa: E402
import data_assets  # noqa: E402
import data_walk  # noqa: E402
import sound_data_assets  # noqa: E402
import sound_music  # noqa: E402
from m68000 import analysis as m68000_analysis  # noqa: E402

ROOT = ea.ROOT
TARGET = ROOT / "runtime" / "src" / "extract_layout.c"
NONE = 0xFFFFFFFF

# the formats of the 68000 program's spans (data_assets.write_piece's order of tests)
FORMATS = ("EXTRACT_FRAME", "EXTRACT_END", "EXTRACT_JUMP", "EXTRACT_SPRITES", "EXTRACT_PALETTE", "EXTRACT_TEXT",
           "EXTRACT_POINTERS", "EXTRACT_WORDS", "EXTRACT_LONGS", "EXTRACT_BYTES")
SOUND_FORMATS = ("EXTRACT_SOUND_ADDRESSES", "EXTRACT_SOUND_WORDS", "EXTRACT_SOUND_BYTES", "EXTRACT_SOUND_MUSIC")

HEADER = """/**
 *    Name:        extract_layout.c
 *    Description: what the first run (extract.c) needs to write the assets extract_assets.py writes, without a
 *                 byte of the ROM
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    {modified}
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Written by tools/extract_layout.py from what the static analysis and the walks find in the player's ROM set:
 *    positions, names, codes and formats only, no byte of the ROM. The sheets of tiles and sprites (their cells'
 *    codes and palettes, the places of the colours they show with), the 68000 program's data pieces (each one's
 *    spans and their formats) and its functions, the sound program's pieces and functions, the songs' files.
 */
"""


def write_format(kind):
    """data_assets.write_piece's format of a span of this kind"""
    if kind in ("animation frame", "animation frame no animation reaches"):
        return 0
    if kind == "animation end":
        return 1
    if kind == "animation jump":
        return 2
    if kind in ("sprite list", "shadow sprite", "sprites no frame shows"):
        return 3
    if kind in ("palette", "palette no list names"):
        return 4
    if kind in data_assets.TEXTS:
        return 5
    if kind in data_assets.POINTER_KINDS:
        return 6
    if kind == "layer map" or "16-bit" in kind or kind == "direction rows":
        return 7
    if "32-bit" in kind:
        return 8
    return 9


def sound_format(name, kind):
    if sound_data_assets.is_music(name, kind):
        return 3
    if kind == "code addresses":
        return 0
    if kind.startswith("u16"):
        return 1
    return 2


def c_string(text):
    if any(character in text for character in '"\\') or not text.isascii() or not text.isprintable():
        raise SystemExit(f"a name this table cannot hold: {text!r}")
    return f'"{text}"'


def palette_sources(walk, area, bank_addresses, sets=()):
    """extract_assets.palette_ram, with the place of the program each colour comes from in place of the colour"""
    mem = walk.mem
    sources = [None] * 1024
    bases = [(mem.r32(bank_addresses + 4 * bank) - 0x80000) // 4 for bank in range(4)]
    for palette_set in [mem.r32(walk.places["area_palettes"] + 4 * area)] + list(sets):
        for bank in range(4):
            listed = mem.r32(palette_set + 4 * bank)
            if not listed:
                continue
            count = (mem.r8(walk.places["palette_bank_counts"] + bank) + 1) * (2 if bank == 3 else 1)
            for index in range(count):
                palette = mem.r32(listed + 4 * index)
                for colour in range(16):
                    entry = bases[bank] + 16 * index + colour
                    if entry < 1024:
                        sources[entry] = palette + 2 * colour
    return sources


def graphics_tables(walk, analysis):
    """[(table name, [(sheet, [(code, slot, unseen)], [16 palette places])])] for the tiles and the sprites, as
    extract_assets.extract makes them"""
    named = {symbol.name: address for address, symbol in analysis.symbols.items()}
    bank_addresses = named["palette_bank_addresses"]
    shown, subject_of, sheet_sets = ea.read_subjects()

    def subject_area(subject):
        area, names = sheet_sets.get(subject, (int(subject.split("_")[1]) if subject.startswith("area_") else 0, []))
        missing = [name for name in names if name not in named]
        if missing:
            raise SystemExit("graphics_subjects.txt: symbols.txt names no " + ", ".join(missing))
        return area, [named[name] for name in names]

    tile_sheets = ea.sheets([(f"area_{area}", uses) for area, uses in ea.area_tiles(walk).items()], shown["tiles"],
                            ea.TILES)
    sprite_sheets = ea.sheets(sorted(ea.animation_groups(walk, analysis).items()), shown["sprites"], ea.SPRITES)
    out = []
    for kind, (sheet_cells, added) in (("tiles", tile_sheets), ("sprites", sprite_sheets)):
        table = []
        for sheet, cells in sheet_cells.items():
            subject = sheet
            if sheet not in sheet_sets and not sheet.startswith("area_"):
                counts = {}
                for code, _ in cells:
                    if code in subject_of[kind]:
                        counts[subject_of[kind][code]] = counts.get(subject_of[kind][code], 0) + 1
                subject = max(counts, key=lambda name: counts[name]) if counts else "area_0"
            area, sets = subject_area(subject)
            sources = palette_sources(walk, area, bank_addresses, sets)
            colours = ea.palette_ram(walk, area, bank_addresses, sets)
            for entry in range(1024):
                expected = ea.asset_formats.rgb555(walk.mem.r16(sources[entry])) if sources[entry] is not None \
                    else (0, 0, 0)
                if colours[entry] != expected:
                    raise SystemExit(f"{sheet}: the palette RAM's entry {entry} is not the colour of its place")
            placed, palette = ea.sheet_colours(cells, sources)
            places = []
            for slot in range(16):
                entries = palette[16 * slot:16 * slot + 16]
                if all(entry is None or entry == (0, 0, 0) for entry in entries):
                    places.append(NONE)
                elif all(isinstance(entry, int) and entry == entries[0] + 2 * index for index, entry in enumerate(entries)):
                    places.append(entries[0])
                else:
                    raise SystemExit(f"{sheet}: its palette {slot} takes colours from more than one place")
            table.append((sheet, [(code, slot, added.get(code) == sheet) for code, slot in placed], places))
        out.append((kind, table))
    return out


def program_tables(image, walk, analysis):
    """The 68000 program's pieces as data_assets.extract makes them: [(name, group, named, [(first, stop, format)])],
    the groups in the order of their files, and {address: function name}"""
    spans = data_assets.partition(image, walk, analysis)
    tables = {address: name for name, address in data_walk.table_places().items()}
    pieces = data_assets.pieces_of(spans, analysis.symbols, image, tables)
    functions = {address: symbol.name for address, symbol in analysis.symbols.items() if symbol.kind in ("func", "code")}
    for address in analysis.functions | analysis.labels:
        functions.setdefault(address, f"func_{address:06X}")
    data_assets.name_pieces(pieces, image, analysis, set(functions.values())
                            | {symbol.name for symbol in analysis.symbols.values()} | set(tables.values()))
    out = [(name, group, True, [(first, stop, write_format(kind)) for first, stop, kind in kinds])
           for name, group, kinds in pieces]
    out += [(name, "code_read", False, [(first, stop, write_format("code read as data"))])
            for first, (stop, name) in sorted(data_assets.CODE_READ_AS_DATA.items())]
    groups = []
    for _, group, _, _ in out:
        if group not in groups:
            groups.append(group)
    return out, groups, functions


def sound_tables(sound):
    program, described = sound_data_assets.description(sound)
    pieces = sound_data_assets.pieces_of(sound, program, described)
    functions = sound_data_assets.function_names(program)
    return pieces, functions


def emit(graphics_out, program, groups, functions, sound_pieces, sound_functions):
    lines = ['#include "extract_layout.h"', ""]
    # the sheets
    for kind, table in graphics_out:
        lines.append(f"/* the {kind}' sheets: their cells (code, palette, taken as a neighbour) */")
        lines.append(f"static const extract_cell {kind}_cells[] = {{")
        for _, cells, _ in table:
            row = []
            for code, slot, unseen in cells:
                row.append(f"{{0x{code:04X}u,{slot},{1 if unseen else 0}}}")
                if len(row) == 8:
                    lines.append("\t" + ", ".join(row) + ",")
                    row = []
            if row:
                lines.append("\t" + ", ".join(row) + ",")
        lines.append("};")
        lines.append(f"const extract_sheet extract_{kind}_sheets[] = {{")
        first = 0
        for sheet, cells, places in table:
            places_text = ", ".join(f"0x{place:06X}u" if place != NONE else "EXTRACT_NO_PLACE" for place in places)
            lines.append(f"\t{{ {c_string(sheet)}, &{kind}_cells[{first}], {len(cells)}u, {{ {places_text} }} }},")
            first += len(cells)
        lines.append("};")
        lines.append(f"const size_t extract_{kind}_sheet_count = {len(table)};")
        lines.append("")
    # the 68000 program's data
    lines.append("/* the 68000 program's data: its files, and every piece's spans (first, stop, format) */")
    lines.append("const char *const extract_data_groups[] = {")
    lines += [f"\t{c_string(group)}," for group in groups]
    lines.append("};")
    lines.append(f"const size_t extract_data_group_count = {len(groups)};")
    lines.append("static const extract_span data_spans[] = {")
    for _, _, _, spans in program:
        for first, stop, kind in spans:
            lines.append(f"\t{{0x{first:06X}u,0x{stop:06X}u,{FORMATS[kind]}}},")
    lines.append("};")
    lines.append("const extract_data_piece extract_data_pieces[] = {")
    first = 0
    for name, group, named, spans in program:
        lines.append(f"\t{{ {c_string(name)}, {groups.index(group)}, {1 if named else 0}, &data_spans[{first}], "
                     f"{len(spans)}u }},")
        first += len(spans)
    lines.append("};")
    lines.append(f"const size_t extract_data_piece_count = {len(program)};")
    lines.append("const extract_function extract_functions[] = {")
    lines += [f"\t{{ 0x{address:06X}u, {c_string(name)} }}," for address, name in sorted(functions.items())]
    lines.append("};")
    lines.append(f"const size_t extract_function_count = {len(functions)};")
    lines.append("")
    # the sound program's
    lines.append("/* the sound program's data: its pieces (name, kind, place; the music's too) and its functions */")
    lines.append("const extract_sound_piece extract_sound_pieces[] = {")
    for name, kind, first, stop in sound_pieces:
        lines.append(f"\t{{ {c_string(name)}, {c_string(kind)}, 0x{first:04X}u, 0x{stop:04X}u, "
                     f"{SOUND_FORMATS[sound_format(name, kind)]} }},")
    lines.append("};")
    lines.append(f"const size_t extract_sound_piece_count = {len(sound_pieces)};")
    lines.append("const extract_function extract_sound_functions[] = {")
    lines += [f"\t{{ 0x{address:04X}u, {c_string(name)} }}," for address, name in sorted(sound_functions.items())]
    lines.append("};")
    lines.append(f"const size_t extract_sound_function_count = {len(sound_functions)};")
    lines.append("")
    # the songs' files
    lines.append("/* the songs table's commands (60 on) the game names: their files and what the game plays them for */")
    lines.append("const extract_song_use extract_song_uses[] = {")
    for command, (name, use) in sorted(sound_music.SONGS.items()):
        lines.append(f"\t{{ 0x{command:02X}, {c_string(name)}, {c_string(use)} }},")
    lines.append("};")
    lines.append(f"const size_t extract_song_use_count = {len(sound_music.SONGS)};")
    lines.append("/* a YM2151 note's velocity by its attenuation (0-127), round(127 * 10 ** (-0.75 * total / 40)) as Python's")
    lines.append("   floating point gives it */")
    lines.append("const uint8_t extract_fm_velocities[128] = {")
    values = [max(1, min(127, round(127 * 10 ** (-0.75 * total / 40)))) for total in range(128)]
    lines += ["\t" + ", ".join(str(value) for value in values[at:at + 16]) + "," for at in range(0, 128, 16)]
    lines.append("};")
    body = "\n".join(lines) + "\n"
    version = int.from_bytes(hashlib.sha256(body.encode("ascii")).digest()[:4], "big")
    return body + f"\n/* the tables' version: the first run's assets say which tables made them */\n" \
                  f"const uint32_t extract_layout_version = 0x{version:08X}u;\n"


def main():
    source = Path(sys.argv[1]) if len(sys.argv) > 1 else ROOT / "rom"
    rom = ea.rom
    sets = rom.load_set(source)
    image = rom.build_program(sets)
    analysis = m68000_analysis.Analysis(image, m68000_analysis.load_symbols(ROOT / "symbols.txt"))
    analysis.run()
    walk = data_walk.Walk(analysis)
    walk.run()
    graphics_out = graphics_tables(walk, analysis)
    program, groups, functions = program_tables(image, walk, analysis)
    sound_pieces, sound_functions = sound_tables(rom.build_sound(sets))
    body = emit(graphics_out, program, groups, functions, sound_pieces, sound_functions)
    old = TARGET.read_text(encoding="ascii") if TARGET.exists() else ""
    spans = sum(len(spans) for _, _, _, spans in program)
    cells = sum(len(cells) for _, table in graphics_out for _, cells, _ in table)
    print(f"{len(program)} pieces of the program's data in {spans} spans, {len(functions)} functions; "
          f"{len(sound_pieces)} of the sound program's, {len(sound_functions)} functions; {cells} cells in "
          f"{sum(len(table) for _, table in graphics_out)} sheets")
    if old.split(" */\n", 1)[-1] == body:
        print(f"{TARGET.relative_to(ROOT)}: as it was")
        return 0
    TARGET.write_text(HEADER.format(modified=date.today().strftime("%d-%m-%Y")) + body, encoding="ascii", newline="\n")
    print(f"{TARGET.relative_to(ROOT)}: written")
    return 0


if __name__ == "__main__":
    sys.exit(main())
