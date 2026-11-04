"""
Name:        extract_assets.py
Description: TMNT's assets from the player's ROM set into common formats, by subject, and back byte for byte
Version:     0.1.0
Created:     29-10-2026
Modified:    04-11-2026
Author:      Sebastian Toyos

Usage: python tools/extract_assets.py [rom folder or .zip] [--check] [--out FOLDER]   (assets/ by default)

Writes the assets the game loads (none of them goes into the repository):
  tiles/    the K052109's tiles in tilesets by subject: first the tiles an area's layers (their maps' blocks, with
            the area's tile banks) and its fire use, each in the first area that uses it; then by
            graphics_subjects.txt (tools/graphics_subjects.py: the fixed layer's character set, the speech
            balloons, and what each scene of the scenarios shows: the opening, an area's story, play and exit,
            the ending...); and a tile none of them names, with its neighbours (with_neighbours). 8-bit
            indexed PNGs, 16 tiles a row: a pixel is the tile's pen plus 16 times the palette it shows with,
            one of the 16 the sheet uses most (an area's blocks say theirs; graphics_subjects.txt the others'),
            with the colours of the sheet's subject (its area's palette set and the ones its scene requests), so
            the pens come back as they are. tilesets.txt lists each sheet's tile codes, in its order, and the
            ones it took as neighbours.
  sprites/  the K051960's 16 x 16 sprites in sheets by the animations that show them (the function whose code
            names the animation or its table), each sprite in the first sheet that uses it; then by
            graphics_subjects.txt, and the rest with their neighbours, as the tiles. The same PNGs, with the
            colours of the subject most of the sheet's sprites show in. sprites.txt lists each sheet's codes.
  sounds/   the title music, the K007232's samples and the uPD7759's speech as WAV files (tools/sound_assets.py)
  data/     the 68000 program's data, every piece by name, as text by subject (tools/data_assets.py)
  sound_data.txt  the Z80 sound program's data by name (tools/sound_data_assets.py), but its music:
  sound/music/    the songs as Standard MIDI Files, a song a file, and music.txt (tools/sound_music.py)
With --check, the tile, sprite and sample ROM files are rebuilt from the assets and compared with the set's,
byte for byte (nothing is written).
"""
import sys
from collections import defaultdict
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path[:0] = [str(TOOLS / "core"), str(TOOLS)]
import game_profile  # noqa: E402  (the game's: its root, its ROM set)
import asset_formats  # noqa: E402  (the core's PNG reader and writer)
import data_assets  # noqa: E402
import data_walk  # noqa: E402
import sound_assets  # noqa: E402
import sound_data_assets  # noqa: E402
import tmnt_graphics as graphics  # noqa: E402
from m68000 import analysis as m68000_analysis  # noqa: E402

ROOT = game_profile.ROOT
rom = game_profile.rom
TILES = 0x100000 // graphics.TILE_BYTES
SPRITES = 0x200000 // graphics.SPRITE_BYTES
COLUMNS = 16
AREAS = 10
SUBJECTS = ROOT / "graphics_subjects.txt"   # what the scenarios show, by subject (tools/graphics_subjects.py)
SPRITE_PALETTES = 256       # the sprites' palettes start at this palette RAM entry (colour base 16)
LAYER_PALETTES = (512, 640)  # layer A's and layer B's (colour bases 32 and 40)
# the sprite sizes (top 3 bits of a sprite list entry): cells across and down, and each cell's code offset
SPRITE_WIDTH = (1, 2, 1, 2, 4, 2, 4, 8)
SPRITE_HEIGHT = (1, 1, 2, 2, 2, 4, 4, 8)
X_OFFSET = (0, 1, 4, 5, 16, 17, 20, 21)
Y_OFFSET = (0, 2, 8, 10, 32, 34, 40, 42)


def ranges(codes):
    """'0010-001F 0123' for the codes in order"""
    parts = []
    for code in codes:
        if parts and parts[-1][1] + 1 == code:
            parts[-1][1] = code
        else:
            parts.append([code, code])
    return " ".join(f"{first:04X}" if first == last else f"{first:04X}-{last:04X}" for first, last in parts)


def parse_ranges(text):
    codes = []
    for part in text.split():
        first, _, last = part.partition("-")
        codes.extend(range(int(first, 16), int(last or first, 16) + 1))
    return codes


# ---- the palettes the sheets show with ----

def palette_ram(walk, area, bank_addresses, sets=()):
    """The palette RAM's colours (1024 of (red, green, blue)) the area's palette set gives, banks 0-3, then the
    palette sets 'sets' (addresses) over it. A colour takes 4 bytes of the 68000's bus (tmnt.cpp's palette: the low
    byte of each word), so a bank's address less 0x80000, divided by 4, is its first colour"""
    mem = walk.mem
    colours = [(0, 0, 0)] * 1024
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
                        colours[entry] = asset_formats.rgb555(mem.r16(palette + 2 * colour))
    return colours


def sheet_colours(cells, colours):
    """A sheet's cells with the palette each shows with (0-63) turned into one of the sheet's 16 (the 16 it uses
    most, in order; another one takes the one it uses most), and the sheet's 256 colours from the palette RAM's"""
    counts = defaultdict(int)
    for _, palette in cells:
        if palette is not None:
            counts[palette] += 1
    kept = sorted(sorted(counts, key=lambda palette: -counts[palette])[:16])
    slot = {palette: index for index, palette in enumerate(kept)}
    most = max(counts, key=lambda palette: counts[palette]) if counts else 0
    out = [(code, slot.get(palette, slot.get(most, 0))) for code, palette in cells]
    palette = [colours[(16 * kept[entry // 16] + entry % 16) % 1024] if entry // 16 < len(kept) else (0, 0, 0)
               for entry in range(256)]
    return out, palette


# ---- what uses which tile and sprite ----

def area_tiles(walk):
    """{area: [(tile code, palette 32-47: layer A's or layer B's)] in the order the area's blocks use them}"""
    mem = walk.mem
    banks_table = walk.places["area_tile_banks"]
    found = {}
    blocks_of = {}
    for first, (stop, kind) in walk.spans.items():
        if kind.startswith("layer blocks"):
            blocks_of[first] = (stop, 33 if "floor" in kind else 17)
    table = walk.places["area_layers"]
    for area in range(AREAS):
        registers = (mem.r8(banks_table + 2 * area), mem.r8(banks_table + 2 * area + 1))
        bank_regs = (registers[0] & 0x0F, registers[0] >> 4, registers[1] & 0x0F, registers[1] >> 4)
        record = table + 36 * area
        uses = []
        layers = [(mem.r32(record + 0x14), 0), (mem.r32(record + 0x1C), 1)]
        if area in (0, 1):
            layers.append((walk.places["fire_blocks"], 1))
        for blocks, layer in layers:
            stop, size = blocks_of.get(blocks, (blocks, 17))
            for block in range(blocks, stop - size + 1, size):
                attribute = mem.r8(block)
                tiles = range(block + size - 16, block + size)
                bank = bank_regs[(attribute & 0x0C) >> 2]
                colour = (attribute & 0xF3) | ((bank & 3) << 2)
                for at in tiles:
                    code = mem.r8(at) | ((colour & 3) << 8) | ((colour & 0x10) << 6) | ((colour & 0x0C) << 9) \
                        | ((bank >> 2) << 13)
                    uses.append((code, LAYER_PALETTES[layer] // 16 + ((colour & 0xE0) >> 5)))
        found[area] = uses
    return found


def animation_groups(walk, analysis):
    """{group name: [(sprite code, palette 16-31)]}: the sprites of the animations each function names"""
    functions = sorted(analysis.functions | {address for address, symbol in analysis.symbols.items()
                                             if symbol.kind == "func"})
    names = {address: name for name, address in data_walk.table_places().items()}
    names.update({address: symbol.name for address, symbol in analysis.symbols.items()})
    from bisect import bisect_right

    def function_of(address):
        index = bisect_right(functions, address) - 1
        start = functions[index] if index >= 0 else 0
        return names.get(start, f"func_{start:06X}")

    namer = {}
    for address, instruction in analysis.instructions.items():
        for value in m68000_analysis.data_places(instruction):
            namer.setdefault(value & 0xFFFFFF, function_of(address))
    # a table of animations no code names is an entry of a table of tables (the turtles' animation sets): it takes
    # the name of the nearest table above it that symbols.txt or the code names; with its index there when that is a
    # table of things (a table line, turtle_animation_sets[4]), not when it is one thing (a data line,
    # leonardo_animation_set)
    arrays = set(data_walk.table_places().values())
    parent = {}
    for table, (depth, entries) in walk.tables.items():
        if depth:
            for index, entry in enumerate(entries):
                parent.setdefault(entry, (table, index))

    def table_group(table):
        child, index = None, None
        while table is not None:
            name = names.get(table) or namer.get(table)
            if name:
                return f"{name}_{index}" if child is not None and table in arrays else name
            child = table
            table, index = parent.get(table, (None, None))
        return None

    owner = {}
    for table, (depth, entries) in walk.tables.items():
        for entry in entries:
            owner.setdefault(entry, namer.get(table) or table_group(table))
    group_of = {animation: namer.get(animation) or owner.get(animation) or names.get(animation)
                for animation in walk.animations}
    # an animation no code, table or name gives a group: the group of the animation whose end jumps to it, else the
    # name of the data that holds its address (a record symbols.txt names: the intro's objects)
    jumped_from = {}
    for animation in sorted(walk.animations):
        read = walk.frames(animation)
        if read is not None and read[1] is not None:
            jumped_from.setdefault(read[1], animation)
    labels = sorted(names)
    holders = defaultdict(list)
    for at in range(0, walk.mem.size - 3, 2):
        holders[walk.mem.r32(at)].append(at)
    for animation in sorted(walk.animations):
        seen = {animation}
        at = animation
        while not group_of.get(at) and at in jumped_from and jumped_from[at] not in seen:
            at = jumped_from[at]
            seen.add(at)
        if group_of.get(at):
            group_of[animation] = group_of[at]
            continue
        for place in holders.get(animation, ()):
            index = bisect_right(labels, place) - 1
            if index >= 0 and (index + 1 == len(labels) or place < labels[index + 1]):
                group_of[animation] = names[labels[index]]
                break
    groups = defaultdict(list)
    for animation in sorted(walk.animations):
        group = group_of.get(animation) or "unnamed"
        read = walk.frames(animation)
        if read is None:
            continue
        for first, length, kind in read[0]:
            if kind != "animation frame":
                continue
            listed = walk.mem.r32(first + 2)
            for sprite in range(walk.mem.r8(first + 1) + 1):
                entry = listed + 8 * sprite
                head, low, colour = walk.mem.r8(entry), walk.mem.r8(entry + 1), walk.mem.r8(entry + 2)
                code = ((head & 0x1F) << 8 | low) | ((colour & 0x10) << 9)
                size = head >> 5
                width, height = SPRITE_WIDTH[size], SPRITE_HEIGHT[size]
                for mask, limit in ((0x01, 2), (0x02, 2), (0x04, 4), (0x08, 4), (0x10, 8), (0x20, 8)):
                    if (mask in (0x01, 0x04, 0x10) and width >= limit) or (mask in (0x02, 0x08, 0x20) and height >= limit):
                        code &= ~mask
                for y in range(height):
                    for x in range(width):
                        groups[group].append((code + X_OFFSET[x] + Y_OFFSET[y], SPRITE_PALETTES // 16 + (colour & 0x0F)))
    return groups


def read_subjects():
    """graphics_subjects.txt (tools/graphics_subjects.py): {'tiles': [(sheet, [(code, palette)])], 'sprites': ...} in
    their order, the subject of the codes the scenarios show and the palette (0-63) each showed with; {'tiles': {code:
    subject}, 'sprites': ...} of every code shown; and {subject: (area, [palette set names])}, its sheet's colours"""
    if not SUBJECTS.exists():
        raise SystemExit(f"{SUBJECTS.name} is missing")
    lines = [line.split(None, 2) for line in SUBJECTS.read_text(encoding="ascii").splitlines()
             if line.strip() and not line.startswith("#")]
    palettes = {"tiles": {}, "sprites": {}}
    for what, palette, codes in lines:
        if what in ("tile_palettes", "sprite_palettes"):
            for code in parse_ranges(codes):
                palettes[what.split("_")[0] + "s"][code] = int(palette)
    found, subject_of, colours = {"tiles": [], "sprites": []}, {"tiles": {}, "sprites": {}}, {}
    for what, sheet, rest in lines:
        if what in found:
            codes = parse_ranges(rest)
            found[what].append((sheet, [(code, palettes[what].get(code)) for code in codes]))
            for code in codes:
                subject_of[what].setdefault(code, sheet)
        elif what == "palettes":
            area, *names = rest.split()
            colours[sheet] = (int(area), names)
    return found, subject_of, colours


def low_zeros(code):
    """How many low bits of a code are 0 (0 has them all)"""
    return (code & -code).bit_length() - 1 if code else 32


def with_neighbours(taken, count):
    """{code: sheet} for the codes no sheet took (no data uses them and no scenario shows them): a run of them
    between two taken codes splits at its most aligned code (the most low zero bits; of those, the nearest the run's
    middle), the codes before it going with the sheet of the code below the run, the rest with the one above's. The
    ROM keeps a subject's graphics together, in blocks of aligned codes: a picture's rows no frame shows, a
    character's frames no animation has"""
    added = {}
    code = 0
    while code < count:
        if code in taken:
            code += 1
            continue
        first = code
        while code < count and code not in taken:
            code += 1
        below, above = taken.get(first - 1), taken.get(code)
        if below is None:
            split = first
        elif above is None:
            split = code
        else:
            middle = (first + code) / 2
            split = max(range(first, code + 1), key=lambda at: (low_zeros(at), -abs(at - middle)))
        for at in range(first, code):
            added[at] = below if at < split else above
    return added


def sheets(first, then, count):
    """{sheet: [(code, palette)]} and {code: sheet} of the codes taken as neighbours: each code in the first sheet
    that uses it, from the subjects 'first' (what the program's data uses, in its order) and 'then' (what the
    scenarios show: a sheet named in both takes both, its own after the data's), the codes none takes with their
    neighbours (with_neighbours). A sheet the data does not order keeps its codes in order"""
    taken, out = {}, {}
    for sheet, uses in list(first) + list(then):
        for code, palette in uses:
            if code not in taken and 0 <= code < count:
                taken[code] = sheet
                out.setdefault(sheet, []).append((code, palette))
    added = with_neighbours(taken, count)
    for code, sheet in sorted(added.items()):
        out[sheet].append((code, None))
    ordered = {sheet for sheet, _ in first}
    for sheet, cells in out.items():
        if sheet not in ordered:
            cells.sort()
    return out, added


# ---- writing and reading the sheets ----

def write_sheet(path, cells, pens_of, size, palette):
    rows_of_cells = (len(cells) + COLUMNS - 1) // COLUMNS
    rows = [[0] * (COLUMNS * size) for _ in range(rows_of_cells * size)]
    for index, (code, colour) in enumerate(cells):
        pens = pens_of(code)
        top, left = (index // COLUMNS) * size, (index % COLUMNS) * size
        for y in range(size):
            for x in range(size):
                rows[top + y][left + x] = pens[y][x] + 16 * colour
    asset_formats.write_png_indexed(path, rows, palette, 8)


def read_sheet(path, count, size):
    rows, _ = asset_formats.read_png_indexed(path)
    cells = []
    for index in range(count):
        top, left = (index // COLUMNS) * size, (index % COLUMNS) * size
        cells.append([[rows[top + y][left + x] & 15 for x in range(size)] for y in range(size)])
    return cells


def extract(source, out):
    sets = rom.load_set(source)
    image = rom.build_program(sets)
    analysis = m68000_analysis.Analysis(image, m68000_analysis.load_symbols(ROOT / "symbols.txt"))
    analysis.run()
    walk = data_walk.Walk(analysis)
    walk.run()
    proms = rom.build_proms(sets)
    tiles = graphics.tiles_for_chips(rom.build_tiles(sets))
    sprites = graphics.sprites_for_chips(rom.build_sprites(sets), proms)
    named = {symbol.name: address for address, symbol in analysis.symbols.items()}
    bank_addresses = named["palette_bank_addresses"]
    shown, subject_of, sheet_sets = read_subjects()

    def subject_colours(subject):
        """The palette RAM a subject's sheet shows with: its area's palette set and the ones its scene requests"""
        area, names = sheet_sets.get(subject, (int(subject.split("_")[1]) if subject.startswith("area_") else 0, []))
        missing = [name for name in names if name not in named]
        if missing:
            raise SystemExit(f"{SUBJECTS.name}: symbols.txt names no " + ", ".join(missing))
        return palette_ram(walk, area, bank_addresses, [named[name] for name in names])

    tile_sheets = sheets([(f"area_{area}", uses) for area, uses in area_tiles(walk).items()], shown["tiles"], TILES)
    sprite_sheets = sheets(sorted(animation_groups(walk, analysis).items()), shown["sprites"], SPRITES)
    for folder, kind, (sheet_cells, added), pens_of, size, listing in (
            ("tiles", "tiles", tile_sheets, lambda code: graphics.tile_pens(tiles, code), 8, "tilesets.txt"),
            ("sprites", "sprites", sprite_sheets, lambda code: graphics.sprite_pens(sprites, code), 16, "sprites.txt")):
        (out / folder).mkdir(parents=True, exist_ok=True)
        for old in (out / folder).glob("*.png"):
            if old.stem not in sheet_cells:
                old.unlink()
        lines = [f"# {folder}: each sheet's codes in the order of its cells, {COLUMNS} a row; before a sheet, the codes",
                 "# it has that no data uses and no scenario shows, taken with their neighbours (tools/extract_assets.py)"]
        for sheet, cells in sheet_cells.items():
            # a sheet shows with its subject's colours: the subject it is, or the one most of its codes show in
            subject = sheet
            if sheet not in sheet_sets and not sheet.startswith("area_"):
                counts = defaultdict(int)
                for code, _ in cells:
                    if code in subject_of[kind]:
                        counts[subject_of[kind][code]] += 1
                subject = max(counts, key=lambda name: counts[name]) if counts else "area_0"
            cells, palette = sheet_colours(cells, subject_colours(subject))
            write_sheet(out / folder / f"{sheet}.png", cells, pens_of, size, palette)
            unseen = [code for code, _ in cells if added.get(code) == sheet]
            if unseen:
                lines.append(f"# {sheet}: {len(unseen)} unseen, {ranges(unseen)}")
            lines.append(f"{sheet}  {ranges([code for code, _ in cells])}")
        (out / folder / listing).write_text("\n".join(lines) + "\n", encoding="ascii")
    pcm, speech = sound_assets.extract(rom, sets, out / "sounds")
    pieces, files = data_assets.extract(image, walk, analysis, out / "data")
    sound_pieces, songs = sound_data_assets.extract(rom.build_sound(sets), out)
    print(f"the program's data: {pieces} pieces in {files} files; the sound program's: {sound_pieces} pieces, the "
          f"music in {songs} MIDI files")
    print(f"tiles: {len(tile_sheets[0])} sheets, sprites: {len(sprite_sheets[0])} sheets, {pcm} K007232 samples, "
          f"{speech} speech samples and the title music in {out}")


def rebuild(folder, listing, count, size, to_bytes, unit):
    data = bytearray(count * unit)
    seen = set()
    for line in (folder / listing).read_text(encoding="ascii").splitlines():
        if not line.strip() or line.startswith("#"):
            continue
        sheet, codes = line.split(None, 1)
        codes = parse_ranges(codes)
        for code, pens in zip(codes, read_sheet(folder / f"{sheet}.png", len(codes), size)):
            data[code * unit:code * unit + unit] = to_bytes(pens)
            seen.add(code)
    missing = count - len(seen)
    return bytes(data), missing


def check(source, out):
    sets = rom.load_set(source)
    proms = rom.build_proms(sets)
    tiles, missing_tiles = rebuild(out / "tiles", "tilesets.txt", TILES, 8, graphics.tile_bytes, graphics.TILE_BYTES)
    sprites, missing_sprites = rebuild(out / "sprites", "sprites.txt", SPRITES, 16, graphics.sprite_bytes,
                                       graphics.SPRITE_BYTES)
    problems = []
    if missing_tiles or graphics.planar_to_chunky(tiles) != rom.build_tiles(sets):
        problems.append(f"the tiles do not come back ({missing_tiles} codes in no sheet)")
    if missing_sprites or graphics.planar_to_chunky(graphics.scramble_sprites(sprites, proms)) != rom.build_sprites(sets):
        problems.append(f"the sprites do not come back ({missing_sprites} codes in no sheet)")
    problems += sound_assets.check(rom, sets, out / "sounds")
    image = rom.build_program(sets)
    analysis = m68000_analysis.Analysis(image, m68000_analysis.load_symbols(ROOT / "symbols.txt"))
    analysis.run()
    problems += data_assets.check(image, analysis, out / "data")
    problems += sound_data_assets.check(rom.build_sound(sets), out)
    print("\n".join(problems) if problems else
          "tiles, sprites, samples, the program's and the sound program's data and the music: the same as the ROM set, "
          "byte for byte")
    return 1 if problems else 0


def main():
    arguments = sys.argv[1:]
    out = ROOT / "assets"
    if "--out" in arguments:
        position = arguments.index("--out")
        out = Path(arguments[position + 1])
        del arguments[position:position + 2]
    checking = "--check" in arguments
    arguments = [argument for argument in arguments if argument != "--check"]
    source = Path(arguments[0]) if arguments else ROOT / "rom"
    if checking:
        return check(source, out)
    extract(source, out)
    return 0


if __name__ == "__main__":
    sys.exit(main())
