"""
Name:        make_scenarios.py
Description: writes the control scripts of tests/controls: the attract mode, the test mode and its sound
             test, games of one and four players, a game with the DIP switches changed, and a four-player
             game in each of the program's ten areas, to its boss's end and, in the last, the ending
Version:     0.1.0
Created:     29-10-2026
Modified:    04-11-2026
Author:      Sebastian Toyos

Usage: python tools/make_scenarios.py

The players do what a script can do without looking: each one walks right, jumps every 12 frames and
attacks every 240. A game starts with a coin and the jump button in a seat (the four-player board has no
start button): Leonardo in the first, then Michelangelo, Donatello and Raphael.

Two places of the work RAM make the rest of the game reachable (found by comparing the RAM of the attract
mode's demos, which play different areas, and of games losing their lives):
  - the area being played, at 060120 (and its copy at 060121): 0 to 9, from the burning building to Krang
    (the areas are listed in AREAS). The program sets it to 0 just after the start and reads it when the
    area loads, some 800 frames later: a poke in between starts the game in another area.
  - each player's lives, at 060101 + 8 x the player's index (2 at the start, with the DIP switches as MAME
    sets them): a poke every 300 frames keeps them up, so a game goes on as long as the script says.
With their lives kept up the players beat every area's boss but Shredder's: in area 2 only if they also walk down
and up the street (a soldier below them holds the scroll), and Shredder and his copies outlast them. A third
place ends the game:
  - the area cleared, at 0600BD: FF is what the game sets once its clear check passes and no turtle is busy, and
    the play goes on to the area's exit; in area 9, to the ending (the epilogue, the staff roll, GAME OVER).
"""
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONTROLS = ROOT / "tests" / "controls"

AREA = 0x060120
AREA_COPY = 0x060121
LIVES = 0x060101                # the first player's; each other player's 8 bytes further
COIN_AT = 700                   # the attract mode's title is on by then
START_AT = 760
AREA_POKE_AT = 800              # after the program sets the area to 0, before it loads it
PLAY_FROM = 900
INTRO_UNTOUCHED_PLAY = 2600    # (the story before area 0 ends around frame 1400 when no button is pressed)
JUMP_EVERY = 12
ATTACK_EVERY = 240
LIVES_EVERY = 300
WANDER_EVERY = 480              # down for WANDER_STEP frames, then up as long, in each of these
WANDER_STEP = 60
# the frame the next area starts at in each area's game, these players beating its boss (measured on build/bin/kref,
# within 1000 frames or 2000 for areas 2 and 4): their games go on 1500 frames past it, for the boss's end and the
# next area's start
AREA_ENDS = {0: 34000, 1: 32000, 2: 62000, 3: 33000, 4: 80000, 5: 55000, 6: 8500, 7: 47000, 8: 36000}
PAST_THE_END = 1500
WANDER_AREAS = {2}              # the areas whose players walk down and up too (play's wander)
AREA_CLEARED = 0x0600BD
# area 9: Krang falls about frame 16000 and Shredder comes; at this frame the area is cleared by a poke and the
# players let go of everything, so that the ending runs as it does with no one at the controls, then GAME OVER and
# the attract mode
ENDING_AT = 30000
ENDING_FRAMES = 10000

AREAS = [
    "the burning building",
    "April's burning apartment",
    "the street",
    "the sewer",
    "the street of the parked cars ('ONE WAY')",
    "the highway",
    "the highway on skateboards",
    "the building site",
    "the Technodrome",
    "Krang's android body and Shredder",
]


def start(lines, players):
    """a coin and the jump button in each seat"""
    for player in players:
        lines.append(f"{COIN_AT + 10 * (player - 1)} press coin{player}")
    for player in players:
        lines.append(f"{START_AT + 10 * (player - 1)} press jump{player}")


def play(lines, players, first, last, keep_lives=False, wander=False):
    """the players walk right, jump and attack from frame 'first' to 'last'; with 'wander', they also walk down
    and up the street in turn (WANDER_EVERY frames, each player a quarter of it later), so that an enemy above or
    below them is in reach too"""
    for player in players:
        lines.append(f"{first} hold right{player}")
    if wander:
        for frame in range(first, last - WANDER_EVERY, WANDER_EVERY):
            for player in players:
                start = frame + (player - 1) * WANDER_EVERY // 4
                lines += [f"{start} hold down{player}", f"{start + WANDER_STEP} release down{player}",
                          f"{start + WANDER_EVERY // 2} hold up{player}",
                          f"{start + WANDER_EVERY // 2 + WANDER_STEP} release up{player}"]
    for frame in range(first, last, JUMP_EVERY):
        for player in players:
            lines.append(f"{frame} press jump{player} 2")
        if (frame - first) % ATTACK_EVERY == ATTACK_EVERY // 2:
            for player in players:
                lines.append(f"{frame} press attack{player} 2")
        if keep_lives and (frame - first) % LIVES_EVERY == 0:
            for player in players:
                lines.append(f"{frame} poke {LIVES + 8 * (player - 1):06X} 02")
    for player in players:
        lines.append(f"{last} release right{player}")


def write(name, header, lines):
    text = "\n".join([f"# {line}" for line in header] + lines) + "\n"
    (CONTROLS / f"{name}.txt").write_text(text, encoding="ascii", newline="\n")


def main():
    CONTROLS.mkdir(parents=True, exist_ok=True)
    write("attract", ["The attract mode from power-on, no input: the RAM and ROM check, the title and its music, the",
                      "turtles' introduction and profiles, the demo game, and the attract mode again"],
          ["7200 end"])

    lines = ["0 dip 3 3 on", "9000 press jump1    # the cross hatch: on to the color test",
             "9300 press jump1    # on to the I/O test, where every input is pressed in turn"]
    frame = 9400
    for player in range(1, 5):
        for control in ("left", "right", "up", "down", "attack") + (("jump",) if player > 1 else ()):
            lines.append(f"{frame} press {control}{player} 8")
            frame += 15
    for control in ("coin1", "coin2", "coin3", "coin4", "service1", "service2", "service3", "service4"):
        lines.append(f"{frame} press {control} 8")
        frame += 15
    lines += [f"{frame + 60} press jump1    # on to the sound test: the first two sounds",
              f"{frame + 120} press attack1", f"{frame + 240} press attack1",
              f"{frame + 400} press jump1    # on to the DIP switch screen",
              f"{frame + 700} press jump1    # back to the mask ROM check", f"{frame + 1000} end"]
    write("test_mode", ["The test mode (DIP switch 3:3 on): the RAM and ROM check and the mask ROM check, then the",
                        "screens the first player's jump button moves through: the cross hatch, the color test, the",
                        "I/O test (every input pressed), the sound test, the DIP switch screen, and the mask ROM check",
                        "again"], lines)

    lines = ["0 dip 3 3 on", "9000 press jump1", "9300 press jump1", "9600 press jump1    # the sound test"]
    frame = 9700
    for _ in range(0x87):
        lines.append(f"{frame} press attack1    # the next sound code")
        frame += 40
    lines.append(f"{frame + 200} end")
    write("sound_test", ["The test mode's sound test: the first player's attack button plays the next code, 00 to 86,",
                         "40 frames each"], lines)

    lines = []
    start(lines, [1])
    play(lines, [1], PLAY_FROM, 2800)
    lines.append("6000 end")
    write("one_player", ["A one-player game: a coin and the jump button, then Leonardo walks right jumping until",
                         "his lives run out, the continue countdown runs out, and the attract mode comes back"], lines)

    lines = []
    start(lines, [1])
    play(lines, [1], INTRO_UNTOUCHED_PLAY, 5000)
    lines.append("6000 end")
    write("intro_untouched", ["A one-player game in which no button is pressed through the story before area 0, to its",
                              "end (the last turtle's landing animation ends on 00 FF, which the sprite engine takes for a",
                              "jump to the long after it: it reads a byte of the boot's code at 001800), then Leonardo",
                              "plays a while"], lines)

    lines = []
    start(lines, [1])
    play(lines, [1], PLAY_FROM, 2800)
    lines += ["3600 press coin1    # during the continue countdown", "3660 press jump1"]
    play(lines, [1], 3800, 7600)
    lines.append("8000 end")
    write("continue", ["A one-player game that continues: a coin and the jump button during the continue countdown"],
          lines)

    lines = []
    start(lines, [1, 2, 3, 4])
    play(lines, [1, 2, 3, 4], PLAY_FROM, 5600)
    lines.append("6000 end")
    write("four_players", ["A four-player game: a coin and the jump button in every seat (Leonardo, Michelangelo,",
                           "Donatello, Raphael), all of them walking right and jumping until their lives run out"],
          lines)

    # the DIP switches away from MAME's defaults (on pulls a line low): 2 coins a credit (SW1 1-3 on), 5 lives
    # (SW2 1-2), the hardest difficulty (SW2 6-7), no demo sounds (SW2 8 off), the screen flipped (SW3 1)
    lines = ["0 dip 1 1 on", "0 dip 1 2 on", "0 dip 1 3 on", "0 dip 2 1 on", "0 dip 2 2 on", "0 dip 2 6 on",
             "0 dip 2 7 on", "0 dip 2 8 off", "0 dip 3 1 on",
             f"{COIN_AT} press coin1", f"{COIN_AT + 30} press coin1    # the second coin makes the credit",
             f"{START_AT + 30} press jump1"]
    play(lines, [1], PLAY_FROM, 9000)
    lines.append("12000 end")
    write("settings", ["The DIP switches away from MAME's defaults: 2 coins a credit, 5 lives, the hardest difficulty,",
                       "no demo sounds, the screen flipped; a one-player game until the lives run out"], lines)

    for number, area in enumerate(AREAS):
        lines = []
        start(lines, [1, 2, 3, 4])
        lines += [f"{AREA_POKE_AT} poke {AREA:06X} {number:02X}    # the game starts in area {number}",
                  f"{AREA_POKE_AT} poke {AREA_COPY:06X} {number:02X}"]
        wander = number in WANDER_AREAS
        if number in AREA_ENDS:
            last = AREA_ENDS[number] + PAST_THE_END
            play(lines, [1, 2, 3, 4], PLAY_FROM, last, keep_lives=True, wander=wander)
            ending = [f"They beat its boss and the next area starts about frame {AREA_ENDS[number]}"]
        else:
            # (area 9: the area cleared during Shredder's fight, then the ending with no one at the controls)
            play(lines, [1, 2, 3, 4], PLAY_FROM, ENDING_AT, keep_lives=True)
            lines.append(f"{ENDING_AT} poke {AREA_CLEARED:06X} FF    # the area cleared: on to the ending")
            last = ENDING_AT + ENDING_FRAMES
            ending = [f"They beat Krang; Shredder outlasts them, so the area is cleared at frame {ENDING_AT} (the area "
                      f"cleared poked at {AREA_CLEARED:06X})", "and they let go of the controls: the ending, the staff "
                      "roll, GAME OVER and the attract mode"]
        lines.append(f"{last + 100} end")
        moves = "walking right, down and up, and jumping" if wander else "walking right and jumping"
        write(f"area{number}", [f"Area {number}, {area}: a four-player game started there (the area poked at "
                                f"{AREA:06X}), the turtles", f"{moves}, their lives kept up"] + ending, lines)


if __name__ == "__main__":
    main()
