"""
Name:        graphics_subjects.py
Description: the subject of each tile and sprite the scenarios show (the scene that shows it first), for the
             assets' sheets: writes graphics_subjects.txt
Version:     0.1.0
Created:     04-11-2026
Modified:    04-11-2026
Author:      Sebastian Toyos

Usage: python tools/graphics_subjects.py [--run]

Reads build/graphics_uses/NAME.txt (kref --graphics-uses for each control script of tests/controls: the tiles and
sprite cells a frame shows for the first time, with the game's state, attract step, scene and area then; --run
makes them first, 6 scripts at a time) and writes graphics_subjects.txt, which tools/extract_assets.py reads to
put the tiles and sprites in sheets by subject: the scene that shows a code, by the game's order (the attract
mode, then each area's story, play and exit, then the continue, the ending, the ranking and the test mode), and
two regions of the K052109's tiles that are what they are wherever they show: the fixed layer's character set and
the speech balloons. A tile or sprite the areas' maps or the animations use already has its sheet (the extractor
puts those first); one no scenario shows goes with its neighbours (the extractor too). It writes too the palette
each code showed with, and the colours of each subject's sheet: its area's palette set and the ones its scene
requests (PALETTE_SETS, by the names symbols.txt gives them).
"""
import subprocess
import sys
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path[:0] = [str(TOOLS / "core"), str(TOOLS)]
import game_profile  # noqa: E402

ROOT = game_profile.ROOT
USES = ROOT / "build" / "graphics_uses"
OUT = ROOT / "graphics_subjects.txt"
JOBS = 6
AREAS = 10
# the attract mode's steps (060003) and the game's scenes (060005), by the names symbols.txt gives their routines
ATTRACT_STEPS = {1: "coin_screen", 2: "opening", 3: "title", 5: "profiles", 6: "best_players"}
DEMO_STEP = 4
SCENES = {1: "choose_turtles", 6: "ending", 7: "continue", 8: "ranking"}
AREA_SCENES = {2: "setup", 3: "story", 4: None, 5: "exit"}     # area N's set-up, story and title, play, exit
# the order a code shown in several scenes goes by: the first of them
ORDER = ["boot", "attract", "coin_screen", "opening", "title", "profiles", "best_players", "new_game", "choose_turtles"]
for _area in range(AREAS):
    ORDER += [f"area_{_area}_setup", f"area_{_area}_story", f"area_{_area}", f"area_{_area}_exit"]
ORDER += ["continue", "ending", "ranking", "test_mode"]
# the palette sets a scene requests for its pictures, over its area's (symbols.txt names them; the instruction that
# requests each): the colours its sheets show with
PALETTE_SETS = {
    "boot": ("boot_palettes",),                             # 0010A2 "lea $a01e.l, A6"
    "coin_screen": ("startup_palettes",),                   # 003FF2 "lea $a05e.l, A6" (startup_begin)
    "title": ("startup_palettes",),
    "profiles": ("startup_palettes",),
    "opening": ("opening_city_palettes",),                  # 02F8F4 "lea $a0ae.l, A5" (opening_city_fade_in)
    "area_0_story": ("intro_palettes",),                    # 030950 "lea $a0ee.l, A6" (start_intro)
    "area_1_exit": ("kidnap_picture_palettes",),            # 02B8E4 "lea $a13e.l, A6" (exit_apartment_shredder_escapes)
    "area_3_exit": ("villains_picture_palettes",),          # 02BD30 "lea $a16e.l, A6"
    "area_4_exit": ("april_hug_palettes",),                 # 02C43A "lea $a11e.l, A6" (exit_car_street_van_drives_off)
    "area_6_exit": ("skateboard_crash_picture_palettes",),  # 02CC8C "lea $a14e.l, A6"
    "area_7_exit": ("splinter_picture_palettes",),          # 02D09A "lea $a12e.l, A6"
    "area_8_exit": ("technodrome_drop_palettes",),          # 02D24A "lea $a15e.l, A6"
    "ending": ("ending_palettes",),                         # 006C3C "lea $a03e.l, A6" (ending_clear)
    "ranking": ("ranking_palettes",),                       # 007B9C "lea $a02e.l, A6" (ranking_clear)
    "test_mode": ("test_mode_palettes",),                   # 0056F2 "lea $a07e.l, A6" (test_start_palette)
}
# regions of the tiles that are one subject wherever they show (each code the areas' maps do not use already)
REGIONS = (
    # the fixed layer's character set: the fonts (a text's tile is its character less 0x30, print_game_text), the
    # Konami logos, GAME OVER, the HUD's frames and digits (rendered); the scenarios show its codes on the fixed
    # layer, and the ranking's digits on layer B
    ("text_and_hud", 0x0000, 0x0200),
    # the speech balloons, a word or two in each (Lights out!, I'll be back, Speedy, Let's move it!: rendered),
    # which the areas, the stories and the exits show on the fixed layer
    ("speech_balloons", 0x1400, 0x1800),
)


def subject(key):
    """The subject of a frame, from kref's key: the game's state, attract step, scene and area"""
    state, step, scene, area = (int(key[at:at + 2], 16) for at in range(0, 8, 2))
    if state == 1:
        if step == DEMO_STEP:
            return f"area_{area}"
        return ATTRACT_STEPS.get(step, "attract")
    if state == 2:
        if scene in AREA_SCENES:
            part = AREA_SCENES[scene]
            return f"area_{area}" + (f"_{part}" if part else "")
        return SCENES.get(scene, "new_game")
    return {0: "boot", 3: "test_mode"}.get(state, "boot")


def run_reference():
    """kref --graphics-uses on every control script"""
    USES.mkdir(parents=True, exist_ok=True)
    reference = ROOT / "build" / "bin" / ("kref.exe" if sys.platform == "win32" else "kref")

    def run(script):
        result = subprocess.run([str(reference), "--script", str(script), "--graphics-uses",
                                 str(USES / f"{script.stem}.txt")], cwd=ROOT, capture_output=True, text=True)
        return script.stem, result.returncode

    with ThreadPoolExecutor(JOBS) as pool:
        for name, code in pool.map(run, sorted((ROOT / "tests" / "controls").glob("*.txt"))):
            if code:
                raise SystemExit(f"kref stopped on {name} (exit {code})")


def ranges(codes):
    parts = []
    for code in sorted(codes):
        if parts and parts[-1][1] + 1 == code:
            parts[-1][1] = code
        else:
            parts.append([code, code])
    return " ".join(f"{first:04X}" if first == last else f"{first:04X}-{last:04X}" for first, last in parts)


def main():
    if "--run" in sys.argv[1:]:
        run_reference()
    paths = sorted(USES.glob("*.txt"))
    if not paths:
        raise SystemExit(f"no kref --graphics-uses files in {USES}")
    # {kind: {code: {subject: the palette it first showed with there}}}
    seen = {"tile": defaultdict(dict), "sprite": defaultdict(dict)}
    for path in paths:
        for line in path.read_text(encoding="ascii").splitlines():
            if line.startswith("#") or not line.strip():
                continue
            parts = line.split()
            palette = int(parts[3] if parts[0] == "tile" else parts[2])
            seen[parts[0]][int(parts[1], 16)].setdefault(subject(parts[-1]), palette)
    rank = {name: index for index, name in enumerate(ORDER)}
    lines = ["# The subject of the tiles and sprites the scenarios show (tools/graphics_subjects.py, from kref",
             "# --graphics-uses on every control script): the scene that shows a code first, by the game's order; for",
             "# the tiles, before them, regions that are one subject wherever they show. tools/extract_assets.py puts",
             "# the tiles the areas' maps use and the sprites the animations show in their sheets first, then these,",
             "# then each code nothing here names with its neighbours. KIND SUBJECT CODES; then the palette (0-63, the",
             "# palette RAM's colour / 16) each code showed with there, PALETTES PALETTE CODES; and the colours of each",
             "# subject's sheet: its area's palette set, then the ones its scene requests, palettes SUBJECT AREA SETS"]
    shown_subjects = set()
    for kind, label in (("tile", "tiles"), ("sprite", "sprites")):
        claimed = set()
        if kind == "tile":
            for name, first, stop in REGIONS:
                lines.append(f"{label}  {name}  {first:04X}-{stop - 1:04X}")
                claimed |= set(range(first, stop))
        by_subject, by_palette = defaultdict(set), defaultdict(set)
        for code, palettes in seen[kind].items():
            first = min(palettes, key=lambda name: rank[name])
            by_palette[palettes[first]].add(code)
            if code not in claimed:
                by_subject[first].add(code)
        for name in ORDER:
            if by_subject.get(name):
                lines.append(f"{label}  {name}  {ranges(by_subject[name])}")
                shown_subjects.add(name)
        for palette in sorted(by_palette):
            lines.append(f"{kind}_palettes  {palette}  {ranges(by_palette[palette])}")
    for name in [name for name, _, _ in REGIONS] + [name for name in ORDER if name in shown_subjects]:
        area = name.split("_")[1] if name.startswith("area_") else "0"
        lines.append(f"palettes  {name}  {area}  {' '.join(PALETTE_SETS.get(name, ()))}".rstrip())
    OUT.write_text("\n".join(lines) + "\n", encoding="ascii")
    print(f"{OUT.name}: {len(seen['tile'])} tiles and {len(seen['sprite'])} sprites shown in {len(paths)} scripts")
    return 0


if __name__ == "__main__":
    sys.exit(main())
