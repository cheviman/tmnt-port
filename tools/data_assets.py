"""
Name:        data_assets.py
Description: the 68000 program's data as text by subject, every piece by name, and back byte for byte
Version:     0.1.0
Created:     29-10-2026
Modified:    29-10-2026
Author:      Sebastian Toyos

For tools/extract_assets.py, which writes them into assets/data. Every byte of the program that is not code is
in a piece: what tools/data_walk.py walks (animations, sprite lists, maps, blocks, palettes, texts...), the
tables symbols.txt describes, the vectors, the fill, and what nothing explains yet. Pieces are the walked spans
of one kind one after the other, named by the symbols.txt name they start at, or by their kind and where the
program had them (a name only: nothing reads that address back but --check). A piece is written as lines of
typed items, which give its bytes back in order:

    u8 0x0A 0x00          bytes              u16 0x1234 ...     words (big endian, as the program)
    s16 -16 5             signed words       u32 0x00010000     longs
    ref NAME[+0xNN]       the address of a piece (or of a function, by its name in symbols.txt or func_XXXXXX)
    frame D N SPRITES BOX ATTACK [A B C D]   an animation frame: its frames, its sprites less one, its sprite
                                             list, its body box offset, its attack box's first word (negative:
                                             none) and the other four when there is one
    end N                 an animation's end: stay on the frame before (0), or a flag the engine never acts on
    jump N TARGET         an animation's end that goes on from the frame TARGET
    sprite CODE COLOUR FLIPS X Y             a sprite of a sprite list (CODE: its size in the top 3 bits)
    colours #RRGGBB ...   palette entries: 5 bits a component (the 3 low bits of each pair of hex digits are 0),
                          '+' after one whose bit 15 is set
    text "..."            characters (\\xNN for a byte that is no printable character)

files: data/<family>.txt, one a family of kinds (animations, sprite lists, layers, palettes, texts, tables...),
and data/layout.txt: each piece's name and the program address it came from, for --check only.
"""
import re
from collections import defaultdict

PER_LINE = {"u8": 16, "u16": 8, "s16": 8, "u32": 4, "ref": 4}
FAMILIES = {
    "animation frame": "animations", "animation frame no animation reaches": "animations", "animation end": "animations", "animation jump": "animations",
    "sprite list": "sprite_lists", "shadow sprite": "sprite_lists", "sprites no frame shows": "sprite_lists",
    "layer map": "layers", "layer blocks": "layers", "layer blocks with floor": "layers",
    "layer blocks no map uses": "layers", "layer blocks with floor no map uses": "layers", "area layers": "layers",
    "palette": "palettes", "palette no list names": "palettes", "palette list": "palettes", "palette set": "palettes",
    "table of palette sets": "palettes", "area palettes": "palettes", "palette bank counts": "palettes",
    "palette colours": "palettes", "palette effect": "palette_effects", "palette effect no list names": "palette_effects", "palette effect list": "palette_effects",
    "palette effect lists": "palette_effects", "table of palette effect lists": "palette_effects",
    "text": "texts", "text table": "texts", "typed text": "texts", "typed text table": "texts",
    "text layer places": "texts", "big font": "texts", "big font place": "texts", "big font tiles": "texts",
    "text colours": "texts", "tile block": "screens", "tile block places": "screens",
    "screen picture tiles": "screens", "screen picture colours": "screens",
    "fire frame": "layers", "fire phase": "layers", "fire phases": "layers",
    "spawn list": "enemies", "area spawn lists": "enemies", "area spawn scrolls": "enemies",
    "area spawn starts": "enemies", "spawn offsets": "enemies", "enemy definitions": "enemies",
    "unused enemy definitions": "enemies", "scroll segment": "scroll", "scroll segment list": "scroll",
    "area scroll segments": "scroll", "demo settings": "demos", "demo inputs": "demos",
    "demo input stream": "demos",
}
# the kinds whose 32-bit entries are all addresses
POINTER_KINDS = ("animation table", "table of animation tables", "table of tables of animation tables",
                 "palette list", "palette set", "table of palette sets", "area palettes", "palette effect lists",
                 "table of palette effect lists", "text table", "typed text table", "area spawn lists",
                 "area scroll segments", "scroll segment list", "demo inputs", "code table", "vectors")
TEXTS = ("text", "typed text")
# the code bytes the program reads as data, by what reads them: the game has no code bytes to read, so each
# is a piece of its own too, over the code it is (coverage.py lists them: "code read as data")
CODE_READ_AS_DATA = {
    # the two flash colours at 04C3EE, which 04C396 reads at 060A5 & 4 ("move.b (A6,D0.w), ($42,A0)"):
    # index 4 is past them, the first byte of the code after them (tst.b's 4A)
    0x04C3F2: (0x04C3F3, "flash_colour_past_its_table"),
    # 048AD2 compares the second nearest player's +4 through A5, which 0483D2 leaves as it found it when it
    # finds one player: in the continue scenario (frame timing, frame 4212) A5 holds 048A94, the handler's
    # own address, and the byte read is its code's (move.l's 2F)
    0x048A98: (0x048A99, "second_player_through_a_stale_pointer"),
    # intro_turtle_lands_animation ends on 00 FF (02DF50), which the sprite engine takes for a jump to the long after
    # it, 00001800 (047134 "move.b ($1,A3), D1", 04713C "movea.l ($2,A3), A3"), and reads the frame's count there
    # (047140 "move.b (A3), D0"): a byte of the boot's "move.w #$3ff, D3" (03). In the intro_untouched scenario, where
    # no button cuts the story before area 0 short (frame 1406)
    0x001800: (0x001801, "intro_turtle_lands_jump_target"),
}


def family(kind):
    if kind in FAMILIES:
        return FAMILIES[kind]
    if "animation table" in kind:
        return "animations"
    return "tables"


def safe(text):
    return re.sub(r"[^0-9A-Za-z_]+", "_", text).strip("_")


class Names:
    """Names of pieces and functions, and back to addresses"""

    def __init__(self, pieces, functions):
        self.starts = sorted(pieces)            # piece start -> (stop, name)
        self.pieces = pieces
        self.functions = functions              # address -> name
        self.addresses = {name: address for address, (_, name) in pieces.items()}
        self.addresses.update({name: address for address, name in functions.items()})
        self.used = set()                       # the functions a piece names

    def name(self, value):
        """NAME or NAME+0xNN for an address a piece or a function holds, else None"""
        if value in self.functions:
            self.used.add(value)
            return self.functions[value]
        from bisect import bisect_right
        index = bisect_right(self.starts, value) - 1
        if index >= 0:
            start = self.starts[index]
            stop, name = self.pieces[start]
            if start <= value < stop:
                return name if value == start else f"{name}+0x{value - start:X}"
        return None

    def value(self, text, where):
        base, _, offset = text.partition("+")
        if base not in self.addresses:
            raise SystemExit(f"{where}: no piece or function named {base}")
        return self.addresses[base] + (int(offset, 16) if offset else 0)


# ---- writing ----

def numbers(kind, values):
    lines = []
    for at in range(0, len(values), PER_LINE[kind]):
        lines.append(kind + " " + " ".join(values[at:at + PER_LINE[kind]]))
    return lines


def write_piece(image, first, stop, kinds, names):
    """The lines of the piece first..stop, made of the spans 'kinds' [(first, stop, kind)]"""
    lines = []
    for start, end, kind in kinds:
        data = image[start:end]
        r16 = lambda at: int.from_bytes(image[at:at + 2], "big")
        r32 = lambda at: int.from_bytes(image[at:at + 4], "big")
        s16 = lambda at: r16(at) - 0x10000 if r16(at) & 0x8000 else r16(at)

        def ref(value):
            name = names.name(value & 0xFFFFFF) if not value >> 24 else None
            return f"ref {name}" if name else f"u32 0x{value:08X}"

        if kind in ("animation frame", "animation frame no animation reaches"):
            text = f"frame {image[start]} {image[start + 1]} {names.name(r32(start + 2)) or hex(r32(start + 2))} " \
                   f"0x{r16(start + 6):04X} {s16(start + 8)}"
            if end - start == 18:
                text += " " + " ".join(str(s16(start + at)) for at in (10, 12, 14, 16))
            lines.append(text)
        elif kind == "animation end":
            lines.append(f"end {image[start + 1]}")
        elif kind == "animation jump":
            lines.append(f"jump {image[start + 1]} {names.name(r32(start + 2)) or hex(r32(start + 2))}")
        elif kind in ("sprite list", "shadow sprite", "sprites no frame shows"):
            for at in range(start, end, 8):
                lines.append(f"sprite 0x{r16(at):04X} 0x{image[at + 2]:02X} 0x{image[at + 3]:02X} {s16(at + 4)} {s16(at + 6)}")
        elif kind in ("palette", "palette no list names"):
            for at in range(start, end - 1, 32):
                colours = []
                for entry in range(at, min(at + 32, end), 2):
                    value = r16(entry)
                    colours.append("#%02X%02X%02X%s" % ((value & 0x1F) << 3, ((value >> 5) & 0x1F) << 3,
                                                        ((value >> 10) & 0x1F) << 3, "+" if value & 0x8000 else ""))
                lines.append("colours " + " ".join(colours))
        elif kind in TEXTS:
            lines.append("text " + text_literal(data))
        elif kind in POINTER_KINDS:
            entries = [ref(r32(at)) for at in range(start, end - 3, 4)]
            for at in range(0, len(entries), 4):
                group = entries[at:at + 4]
                if all(entry.startswith("ref ") for entry in group):
                    lines.append("ref " + " ".join(entry[4:] for entry in group))
                else:
                    lines += group
            if (end - start) % 4:
                lines += numbers("u8", [f"0x{byte:02X}" for byte in data[(end - start) // 4 * 4:]])
        elif kind == "layer map" or "16-bit" in kind or kind == "direction rows":
            lines += numbers("u16", [f"0x{r16(at):04X}" for at in range(start, end - 1, 2)])
            if (end - start) % 2:
                lines.append(f"u8 0x{image[end - 1]:02X}")
        elif "32-bit" in kind:
            items = [ref(r32(at)) for at in range(start, end - 3, 4)]
            lines += items
        else:
            lines += numbers("u8", [f"0x{byte:02X}" for byte in data])
    return lines


def text_literal(data):
    out = []
    for byte in data:
        if 0x20 <= byte < 0x7F and byte not in (0x22, 0x5C):
            out.append(chr(byte))
        else:
            out.append(f"\\x{byte:02X}")
    return '"' + "".join(out) + '"'


# ---- reading ----

def read_piece(lines, names, where):
    out = bytearray()
    for number, line in enumerate(lines):
        here = f"{where}, line {number + 1}"
        token, _, rest = line.partition(" ")
        parts = rest.split()
        if token == "u8":
            out += bytes(int(value, 16) for value in parts)
        elif token == "u16":
            out += b"".join(int(value, 16).to_bytes(2, "big") for value in parts)
        elif token == "s16":
            out += b"".join((int(value) & 0xFFFF).to_bytes(2, "big") for value in parts)
        elif token == "u32":
            out += b"".join(int(value, 16).to_bytes(4, "big") for value in parts)
        elif token == "ref":
            out += b"".join(names.value(value, here).to_bytes(4, "big") for value in parts)
        elif token == "frame":
            duration, count, sprites, box, attack = parts[:5]
            target = names.value(sprites, here) if not sprites.startswith("0x") else int(sprites, 16)
            out += bytes([int(duration), int(count)]) + target.to_bytes(4, "big") + int(box, 16).to_bytes(2, "big")
            out += (int(attack) & 0xFFFF).to_bytes(2, "big")
            out += b"".join((int(value) & 0xFFFF).to_bytes(2, "big") for value in parts[5:])
        elif token == "end":
            out += bytes([0, int(parts[0])])
        elif token == "jump":
            target = names.value(parts[1], here) if not parts[1].startswith("0x") else int(parts[1], 16)
            out += bytes([0, int(parts[0])]) + target.to_bytes(4, "big")
        elif token == "sprite":
            code, colour, flips, x, y = parts
            out += int(code, 16).to_bytes(2, "big") + bytes([int(colour, 16), int(flips, 16)])
            out += (int(x) & 0xFFFF).to_bytes(2, "big") + (int(y) & 0xFFFF).to_bytes(2, "big")
        elif token == "colours":
            for colour in parts:
                red, green, blue = (int(colour[1 + 2 * index:3 + 2 * index], 16) >> 3 for index in range(3))
                out += ((0x8000 if colour.endswith("+") else 0) | blue << 10 | green << 5 | red).to_bytes(2, "big")
        elif token == "text":
            literal = rest.strip()[1:-1]
            at = 0
            while at < len(literal):
                if literal[at] == "\\":
                    out.append(int(literal[at + 2:at + 4], 16))
                    at += 4
                else:
                    out.append(ord(literal[at]))
                    at += 1
        else:
            raise SystemExit(f"{here}: '{token}' is no item of a piece")
    return bytes(out)


# ---- the pieces ----

def partition(image, walk, analysis):
    """[(first, stop, kind)] of every byte of the program that is no code, in order"""
    spans = [(first, stop, kind) for first, (stop, kind) in walk.spans.items()]
    taken = set()
    for first, stop, _ in spans:
        taken.update(range(first, stop))
    extra = []
    for first, stop in analysis.described:
        kind = "vectors" if first == 0 else "code table" if first in analysis.table_ends else "described table"
        extra.append((first, stop, kind))
    for first, stop in getattr(analysis, "fill", []):
        extra.append((first, stop, "fill"))
    for first, stop, kind in extra:
        free = [address for address in range(first, stop) if address not in taken and address not in analysis.owner]
        if free:
            # (one span per run of what is free in it)
            run = [free[0]]
            for address in free[1:] + [None]:
                if address is not None and address == run[-1] + 1:
                    run.append(address)
                    continue
                spans.append((run[0], run[-1] + 1, kind))
                taken.update(run)
                if address is not None:
                    run = [address]
    # what nothing explains: runs of the bytes left, not code
    run = []
    for address in range(len(image)):
        if address in taken or address in analysis.owner:
            if run:
                spans.append((run[0], run[-1] + 1, "unexplained"))
                run = []
            continue
        run.append(address)
    if run:
        spans.append((run[0], run[-1] + 1, "unexplained"))
    return sorted(spans)


def pieces_of(spans, symbols, image, tables=None):
    """[(name, family, [(first, stop, kind)])]: the spans of a family one right after the other, one piece,
    broken where symbols.txt names a place (a data line, or a table line in 'tables', {address: name}); a name of
    None for a piece symbols.txt does not name (name_pieces). A byte of 00 or FF that nothing explains, right after
    a piece and before an even address, is that piece's: the fill that puts what follows at an even address"""
    named = {address: symbol.name for address, symbol in symbols.items()}
    named.update({address: name for address, name in (tables or {}).items() if address not in named})
    out = []
    for first, stop, kind in spans:
        group = family(kind)
        if kind == "unexplained" and stop == first + 1 and stop % 2 == 0 and image[first] in (0x00, 0xFF) \
                and out and out[-1][2][-1][1] == first and first not in named:
            out[-1][2].append((first, stop, "alignment"))
            continue
        if out and out[-1][1] == group and out[-1][2][-1][1] == first and first not in named:
            out[-1][2].append((first, stop, kind))
            continue
        out.append([named.get(first), group, [(first, stop, kind)]])
    return out


# the word a piece's name ends with, after what names it, by its family (a table of routines: 'routines')
ROLES = {"tables": "table", "texts": "text", "sprite_lists": "sprites", "animations": "animation",
         "layers": "layer", "palettes": "palettes", "palette_effects": "palette_effects", "screens": "picture",
         "enemies": "enemies", "scroll": "scroll", "demos": "demo"}


def name_pieces(pieces, image, analysis, taken):
    """Names for the pieces symbols.txt does not name, by what names them in the program: the function whose
    instructions name the piece's address ('title_setup_routines'), else the named piece that holds its address
    (an entry of a table, 'game_texts_5'; an animation's frame, 'enemy_hop_animation_sprites'), taken again until
    nothing more gets a name; the rest by its kind and where the program had it. 'taken' are the names already in
    use (functions, symbols.txt's), which a piece's name does not repeat"""
    from bisect import bisect_right
    from m68000 import analysis as m68000_analysis
    names = {address: symbol.name for address, symbol in analysis.symbols.items()}
    functions = sorted(analysis.functions | {address for address, symbol in analysis.symbols.items()
                                             if symbol.kind == "func"})
    used = set(taken) | {piece[0] for piece in pieces if piece[0]}

    def unique(name):
        name = safe(name)
        candidate, count = name, 1
        while candidate in used:
            count += 1
            candidate = f"{name}_{count}"
        used.add(candidate)
        return candidate

    def role(group, kinds):
        return "routines" if kinds[0][2] == "code table" else ROLES.get(group, safe(group))

    # the functions whose instructions name an address
    readers = defaultdict(set)
    for address, instruction in analysis.instructions.items():
        for value in m68000_analysis.data_places(instruction):
            index = bisect_right(functions, address) - 1
            if index >= 0 and functions[index] in names:
                readers[value & 0xFFFFFF].add(names[functions[index]])
    for piece in pieces:
        name, group, kinds = piece
        first, stop = kinds[0][0], kinds[-1][1]
        if name is None and first == 0:
            piece[0] = unique("vectors")
        elif name is None and kinds[0][2] == "unexplained" and stop == len(image) and set(image[first:stop]) == {0xFF}:
            piece[0] = unique("fill_to_the_program_end")
        elif name is None and readers.get(first):
            piece[0] = unique(f"{sorted(readers[first])[0]}_{role(group, kinds)}")
        elif name is None and kinds[0][2] == "code table":
            # a table of routines no instruction names (an object's, through object_routine_tables): by the words its
            # routines' names begin with, when they share some
            routines = [names.get(int.from_bytes(image[at:at + 4], "big") & 0xFFFFFF)
                        for at in range(kinds[0][0], kinds[0][1] - 3, 4)]
            words = [routine.split("_") for routine in routines if routine]
            prefix = []
            for parts in zip(*words) if words else ():
                if len(set(parts)) != 1:
                    break
                prefix.append(parts[0])
            if prefix and len(words) > 1:
                piece[0] = unique("_".join(prefix) + "_routines")
    # the longs that hold a piece's address, where they are
    holders = defaultdict(list)
    for at in range(0, len(image) - 3, 2):
        holders[int.from_bytes(image[at:at + 4], "big")].append(at)
    spans = sorted((kind_first, kind_stop, kind, piece) for piece in pieces for kind_first, kind_stop, kind in piece[2])
    span_starts = [span[0] for span in spans]
    changed = True
    while changed:
        changed = False
        for piece in pieces:
            name, group, kinds = piece
            if name is not None:
                continue
            for place in holders.get(kinds[0][0], ()):
                index = bisect_right(span_starts, place) - 1
                if index < 0 or not spans[index][0] <= place < spans[index][1]:
                    continue
                first, stop, kind, holder = spans[index]
                if holder[0] is None or holder is piece:
                    continue
                start = holder[2][0][0]
                if kind in ("animation frame", "animation frame no animation reaches"):
                    piece[0] = unique(f"{holder[0]}_{role(group, kinds)}")
                elif kind in POINTER_KINDS or "32-bit" in kind or family(kind) == "tables":
                    piece[0] = unique(f"{holder[0]}_{(place - start) // 4}")
                else:
                    piece[0] = unique(f"{holder[0]}_{role(group, kinds)}")
                changed = True
                break
    # a piece right after a named one, which nothing else names: the rest of that one, where the walk took its other
    # entries for another kind (intro_rooftop_animations: its first entry, then three)
    for before, piece in zip(pieces, pieces[1:]):
        if piece[0] is None and before[0] is not None and before[2][-1][1] == piece[2][0][0]:
            piece[0] = unique(f"{before[0]}_rest")
    for piece in pieces:
        if piece[0] is None:
            name, group, kinds = piece
            piece[0] = unique(f"{safe(group if group != 'tables' else kinds[0][2])}_{kinds[0][0]:06X}")
    return pieces


def extract(image, walk, analysis, folder):
    folder.mkdir(parents=True, exist_ok=True)
    import data_walk
    spans = partition(image, walk, analysis)
    tables = {address: name for name, address in data_walk.table_places().items()}
    pieces = pieces_of(spans, analysis.symbols, image, tables)
    functions = {address: symbol.name for address, symbol in analysis.symbols.items() if symbol.kind in ("func", "code")}
    for address in analysis.functions | analysis.labels:
        functions.setdefault(address, f"func_{address:06X}")
    name_pieces(pieces, image, analysis, set(functions.values()) | {symbol.name for symbol in analysis.symbols.values()}
                | set(tables.values()))
    names = Names({kinds[0][0]: (kinds[-1][1], name) for name, _, kinds in pieces}, functions)
    files = defaultdict(list)
    layout = ["# each piece and where the program had it (for --check)"]
    # (a family of their own, code_read: the C reaches them only through the pointers that lead there, so no number
    # takes their names: the profile's PIECE_GROUPS_UNNAMED)
    pieces += [[name, "code_read", [(first, stop, "code read as data")]]
               for first, (stop, name) in sorted(CODE_READ_AS_DATA.items())]
    for name, group, kinds in pieces:
        files[group].append(f"[{name}]")
        files[group] += ["  " + line for line in write_piece(image, kinds[0][0], kinds[-1][1], kinds, names)]
        layout.append(f"{name} {kinds[0][0]:06X} {group}")
    for group, lines in files.items():
        (folder / f"{group}.txt").write_text(f"# {group}: pieces of the program's data, by name\n"
                                             + "\n".join(lines) + "\n", encoding="ascii")
    (folder / "layout.txt").write_text("\n".join(layout) + "\n", encoding="ascii")
    # the code addresses the data holds, by name (the vectors, the tables of handlers)
    (folder / "functions.txt").write_text("# the program's places in its code the data names, and their addresses\n"
                                          + "".join(f"{name} {address:06X}\n" for address, name in sorted(functions.items())
                                                    if address in names.used),
                                          encoding="ascii")
    return len(pieces), len(files)


def check(image, analysis, folder):
    """What does not come back: every piece rebuilt at its address, compared with the program's bytes"""
    layout = {}
    for line in (folder / "layout.txt").read_text(encoding="ascii").splitlines():
        if line and not line.startswith("#"):
            name, address, group = line.split()
            layout[name] = (int(address, 16), group)
    texts = {}
    for group in {group for _, group in layout.values()}:
        current = None
        for line in (folder / f"{group}.txt").read_text(encoding="ascii").splitlines():
            if line.startswith("["):
                current = line[1:-1]
                texts[current] = []
            elif line.startswith("  ") and current:
                texts[current].append(line.strip())
    functions = {address: symbol.name for address, symbol in analysis.symbols.items() if symbol.kind in ("func", "code")}
    for address in analysis.functions | analysis.labels:
        functions.setdefault(address, f"func_{address:06X}")
    names = Names({address: (address, name) for name, (address, _) in layout.items()}, functions)
    rebuilt = bytearray(image)
    problems = []
    for name, (address, group) in layout.items():
        data = read_piece(texts.get(name, []), names, f"{group}.txt [{name}]")
        rebuilt[address:address + len(data)] = data
        if image[address:address + len(data)] != data:
            problems.append(f"{name} ({group}) does not come back")
    return problems[:20] + ([f"... and {len(problems) - 20} more"] if len(problems) > 20 else [])
