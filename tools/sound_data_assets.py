"""
Name:        sound_data_assets.py
Description: the Z80 sound program's data as text by name, the music as MIDI files, and back byte for byte
Version:     0.1.0
Created:     29-10-2026
Modified:    04-11-2026
Author:      Sebastian Toyos

For tools/extract_assets.py, which writes assets/sound_data.txt and assets/sound/music. Every byte of the sound
program that is not code is in a piece: what sound_symbols.txt describes and the music tools/sound_data_walk.py
walks (tools/core/sound_coverage.py's description), the fill, and what nothing explains (none today). The music
(the tracks and their parts) and the songs table are the songs' Standard MIDI Files, a song a file
(tools/sound_music.py: assets/sound/music, music.txt the files). Every other piece is written in sound_data.txt
as lines of items, which give its bytes back in order (the Z80 is little endian):

    u8 0x0A 0x00          bytes              u16 0x1234 ...     words
    ref NAME[+0xNN]       the 16-bit address of a piece or of a function (by its sound_symbols.txt name, or
                          sound_XXXX)

and assets/sound_layout.txt says where each piece was, the music's too: the addresses the program's pointers
hold, which the game's loader gives each piece by its name.
"""
import re
from bisect import bisect_right
from collections import defaultdict
from pathlib import Path

import sound_music

SONGS_TABLE = "songs"           # the songs table's piece: the MIDI files give it back


def description(image):
    """The core's sound coverage's description of the program (sound_symbols.txt and the music's walk)"""
    import sound_coverage
    program = sound_coverage.analysis.load_program(image, sound_coverage.SYMBOLS)
    names = sound_coverage.disassemble.load_names(sound_coverage.SYMBOLS, sound_coverage.REGISTERS)
    tables, fills = sound_coverage.load_lines(sound_coverage.SYMBOLS)
    return program, sound_coverage.Description(program, names, tables, fills)


class Names:
    def __init__(self, pieces, functions):
        self.starts = sorted(pieces)
        self.pieces = pieces                    # start -> (stop, name)
        self.functions = functions              # address -> name
        self.addresses = {name: start for start, (_, name) in pieces.items()}
        self.addresses.update({name: address for address, name in functions.items()})

    def name(self, value):
        if value in self.functions:
            return self.functions[value]
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


def pieces_of(image, program, described):
    """[(name, kind, first, stop)] of every byte that is no code, in order (the music's runs merged by track)"""
    code = set()
    for address, instruction in program.analysis.instructions.items():
        code.update(range(address, address + instruction.length))
    found, taken = [], set()
    # code the program also reads as data (codedata lines: the main loop sums 0000-00FF into its random bytes):
    # a piece of its own, code bytes and all, which the C reads where the program's pointer says
    for piece in described.pieces:
        if getattr(piece, "code_read", False):
            span = range(piece.first, min(piece.stop, len(image)))
            found.append((piece.name, piece.kind, piece.first, span.stop))
            taken.update(span)
    for piece in sorted(described.pieces, key=lambda piece: (piece.first, piece.stop)):
        bytes_free = [address for address in range(piece.first, min(piece.stop, len(image)))
                      if address not in code and address not in taken]
        if not bytes_free:
            continue
        run = [bytes_free[0]]
        for address in bytes_free[1:] + [None]:
            if address is not None and address == run[-1] + 1:
                run.append(address)
                continue
            kind = piece.kind
            found.append((by_subject(piece.name), kind, run[0], run[-1] + 1))
            taken.update(run)
            if address is not None:
                run = [address]
    rest = [address for address in range(program.code_end) if address not in code and address not in taken]
    run = []
    for address in rest + [None]:
        if run and (address is None or address != run[-1] + 1):
            last = run[-1] + 1
            whole_fill = last == program.code_end and set(image[run[0]:last]) == {0xFF}
            found.append(("fill_to_the_program_end" if whole_fill else f"unexplained_{run[0]:04X}", "unexplained",
                          run[0], last))
            run = []
        if address is not None:
            run.append(address)
    names = defaultdict(int)
    unique = []
    found = [(re.sub(r"[^0-9A-Za-z_]+", "_", name).strip("_"), kind, first, stop) for name, kind, first, stop in found]
    # a name that comes back (a piece split by code, a part a song plays twice) takes its number: _2, _3...
    for name, kind, first, stop in sorted(found, key=lambda piece: piece[2]):
        names[name] += 1
        unique.append((name if names[name] == 1 else f"{name}_{names[name]}", kind, first, stop))
    return unique


# the tables of pointers whose records are a subject of their own, by the name of their entries
HOLDER_NAMES = {"fm_voice_pointers": "fm_instrument"}


def by_subject(name):
    """A record's piece by what reaches it, not by its address: the core's coverage names one 'STRUCT ADDRESS
    (SOURCE)', SOURCE the table entry that points at it (fm_voice_pointers[3]: 'fm_instrument_3') or the functions that
    take it ('of fm_effect_01_note': 'fm_effect_01_note_voice')"""
    match = re.fullmatch(r"(\w+) [0-9A-F]{4} \((.*)\)", name)
    if not match:
        return name.replace("'s part", " part")
    struct, source = match.groups()
    short = struct.split("_")[-1]
    entry = re.fullmatch(r"(\w+)\[(\d+)\]", source)
    if entry:
        holder, index = entry.groups()
        return f"{HOLDER_NAMES[holder]}_{index}" if holder in HOLDER_NAMES else f"{holder}_{index}_{short}"
    if source.startswith("of "):
        # (by the first of the functions that take it: others share it)
        return f"{re.split(r'[ ,]+', source[3:])[0]}_{short}"
    return f"{source}_{short}"


def write_piece(image, kind, first, stop, names):
    lines = []
    if kind == "code addresses":
        for at in range(first, stop - 1, 2):
            value = image[at] | image[at + 1] << 8
            name = names.name(value)
            lines.append(f"ref {name}" if name else f"u16 0x{value:04X}")
        if (stop - first) % 2:
            lines.append(f"u8 0x{image[stop - 1]:02X}")
    elif kind.startswith("u16"):
        values = [f"0x{image[at] | image[at + 1] << 8:04X}" for at in range(first, stop - 1, 2)]
        lines += ["u16 " + " ".join(values[at:at + 8]) for at in range(0, len(values), 8)]
        if (stop - first) % 2:
            lines.append(f"u8 0x{image[stop - 1]:02X}")
    else:
        values = [f"0x{byte:02X}" for byte in image[first:stop]]
        lines += ["u8 " + " ".join(values[at:at + 16]) for at in range(0, len(values), 16)]
    return lines


def read_piece(lines, names, where):
    out = bytearray()
    for number, line in enumerate(lines):
        here = f"{where}, line {number + 1}"
        token, _, rest = line.partition(" ")
        parts = rest.split()
        if token == "u8":
            out += bytes(int(value, 16) for value in parts)
        elif token == "u16":
            out += b"".join(int(value, 16).to_bytes(2, "little") for value in parts)
        elif token == "ref":
            out += names.value(parts[0], here).to_bytes(2, "little")
        else:
            raise SystemExit(f"{here}: '{token}' is no item of a piece")
    return bytes(out)


def function_names(program):
    found = {address: symbol.name for address, symbol in program.symbols.items() if symbol.kind in ("func", "code", "loop")}
    for address in program.analysis.instructions:
        pass
    for address in getattr(program, "entries", {}) or {}:
        found.setdefault(address, f"sound_{address:04X}")
    return found


def is_music(name, kind):
    """A piece the songs' MIDI files give back: a track or a part, or the songs table"""
    return kind.startswith("music") or name == SONGS_TABLE


def extract(image, out):
    """sound_data.txt, sound_layout.txt and sound_functions.txt, and the songs' MIDI files (sound/music); returns
    the pieces' count and the files'"""
    program, described = description(image)
    pieces = pieces_of(image, program, described)
    functions = function_names(program)
    names = Names({first: (stop, name) for name, _, first, stop in pieces}, functions)
    lines = ["# the sound program's data: its pieces by name (the music and the songs table: sound/music)"]
    layout = ["# each piece and where the sound program had it: the address the game gives it"]
    for name, kind, first, stop in pieces:
        layout.append(f"{name} {first:04X}")
        if is_music(name, kind):
            continue
        lines.append(f"[{name}]  # {kind}")
        lines += ["  " + line for line in write_piece(image, kind, first, stop, names)]
    (out / "sound_data.txt").write_text("\n".join(lines) + "\n", encoding="ascii")
    (out / "sound_layout.txt").write_text("\n".join(layout) + "\n", encoding="ascii")
    (out / "sound_functions.txt").write_text("# the sound program's places in its code the data names, and their addresses\n"
                                             + "".join(f"{name} {address:04X}\n" for address, name in sorted(functions.items())),
                                             encoding="ascii")
    files, _ = sound_music.write(image, pieces, out / "sound" / "music")
    return len(pieces), files


def check(image, out):
    """What does not come back the same as the sound program 'image': the text's pieces, and the music and the
    songs table from the MIDI files"""
    program, described = description(image)
    layout = {}
    for line in (out / "sound_layout.txt").read_text(encoding="ascii").splitlines():
        if line and not line.startswith("#"):
            name, address = line.split()
            layout[name] = int(address, 16)
    texts, current = {}, None
    for line in (out / "sound_data.txt").read_text(encoding="ascii").splitlines():
        if line.startswith("["):
            current = line[1:line.index("]")]
            texts[current] = []
        elif line.startswith("  ") and current:
            texts[current].append(line.strip())
    names = Names({address: (address, name) for name, address in layout.items()}, function_names(program))
    problems = []
    for name, address in layout.items():
        if name in texts:
            data = read_piece(texts[name], names, f"sound_data.txt [{name}]")
            if image[address:address + len(data)] != data:
                problems.append(f"the sound program's {name} does not come back")
    # the music: every byte of its pieces, from the MIDI files
    places = sorted(layout.values())
    stops = {address: (places[index + 1] if index + 1 < len(places) else program.code_end)
             for index, address in enumerate(places)}
    music_names = sound_music.Names({address: (stops[address], name) for name, address in layout.items()})
    lengths = read_piece(texts.get("note_lengths", []), names, "sound_data.txt [note_lengths]")
    try:
        rebuilt = sound_music.read(out / "sound" / "music", music_names, lengths, layout[SONGS_TABLE])
    except sound_music.MusicError as error:
        return problems[:19] + [f"the music: {error}"]
    music = [(name, first, stop) for name, kind, first, stop in pieces_of(image, program, described)
             if is_music(name, kind)]
    inside = set()
    for _, first, stop in music:
        inside.update(range(first, stop))
    outside = sorted(set(rebuilt.bytes) - inside)
    if outside:
        problems.append(f"the MIDI files give {len(outside)} bytes outside the music, the first at "
                        f"{music_names.name(outside[0])} ({rebuilt.bytes[outside[0]][1]})")
    for name, first, stop in music:
        if name not in layout or name in texts:
            problems.append(f"the sound program's {name} is not the MIDI files' (sound_layout.txt, sound_data.txt)")
            continue
        missing = [address for address in range(first, stop) if address not in rebuilt.bytes]
        differ = [address for address in range(first, stop)
                  if address in rebuilt.bytes and rebuilt.bytes[address][0] != image[address]]
        if missing:
            problems.append(f"the sound program's {name}: no MIDI file gives {music_names.name(missing[0])} "
                            f"({len(missing)} bytes)")
        if differ:
            problems.append(f"the sound program's {name} does not come back from the MIDI files "
                            f"({music_names.name(differ[0])}, {rebuilt.bytes[differ[0]][1]})")
    return problems[:20]
