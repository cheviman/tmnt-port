# TMNT — how the port is made

The port's own pieces, for those who build or change it: how it runs the decompiled game, its first run, the
packages and the console version's tools. How each device is built and packed is in `ports/PLAN.md`, the console
version's plan and what was done in `console/PLAN.md`, and the decompilation itself (its status, the tools, the
assets' formats, the checks against MAME) in [tmntarcade-decomp](https://github.com/cheviman/tmntarcade-decomp)'s
README.

## How the port works

- **Game code.** The 68000 and Z80 machine code is decompiled to C (`src/`, `src/sound/`), so the game runs
  natively with no CPU emulation. `runtime/src/game_program.c` runs it by frames: the main loop, the VBLANK
  interrupt as a function, the sound board's frames on its own timing.
- **Data.** The game loads its assets (`runtime/src/assets.c`, `music.c`): the sheets into the video chips'
  memories, the samples into the sound chips', and the programs' data, piece by piece, by name; the numbers the
  original keeps in its state (addresses, tile and sprite codes) are resolved through the assets' index. It
  opens no ROM.
- **Video and sound.** The K052109, K051960 and the palette are a library (`runtime/src/video.c`) that draws each
  frame from the board's memories into a 320 x 224 picture; the YM2151, K007232, uPD7759 and the title music's
  sample are libraries that make the samples a platform plays.
- **Platforms.** A port provides a window, input, audio output and the frame's pace, through the board's hooks
  (`runtime/include/tmnt/machine.h`); `ports/sdl2/` is the first.

## TMNT Arcade's options

Options of `TMNT Arcade` and `tmnt_game_headless`: `--script FILE` plays a control script (format in
`runtime/include/tmnt/script.h`) instead of the controls (`--fast` in the window runs it without waiting for each
frame's time); `--hashes`, `--video-hashes` and `--sound-hashes FILE` write a hash of every frame's state,
picture and sound, as the reference does; `--service` turns the test mode's DIP switch on; `--assets FOLDER`
says where the assets are (a folder, or a pack: a file whose name ends in `.assets`); `--write-pack FILE` writes
the pack of the assets loaded and stops. `tmnt_game_headless` also takes `--frames N`, `--dump FRAME --dump-dir
DIR` (the board's memories at the end of a frame) and `--video-dump FRAME ... --video-dir DIR` (those frames' pictures as BMP,
as the game's own video draws them: the screenshots above).

## The port's first run

A player of the port has no Python: with no assets, `TMNT Arcade` takes them out of the ROM set itself, once.

- **Where.** It loads the pack `tmnt.assets` or the folder `assets` beside the executable, in the folder where
  the game keeps its files (beside the executable on Windows; `~/.local/share/TMNT` on Linux,
  `~/Library/Application Support/TMNT` on macOS) or in the current folder; `--assets` picks one. With none, it
  looks for the ROM set, `tmnt.zip` or a folder `rom/` with its files (or its .zip), beside the executable or in
  the game's folder; with none there, a window asks for it to be dropped on it and says what it is doing and
  what failed. The assets go into `assets/` in the game's folder, and the game plays.
- **The same files.** The first run (`runtime/src/extract.c` and the files beside it) writes the folder
  `tools/extract_assets.py` writes, every file byte for byte: the sheets' PNGs packed as zlib's level 9 packs them
  (`deflate.c`), the WAVs, the data as text, the songs' MIDI files (`extract_music.c`, `tools/sound_music.py`'s
  writer in C). What the Python tools work out from the static analysis (which bytes are which piece, their names
  and formats, the sheets' codes and palettes) it takes from `runtime/src/extract_layout.c`, which
  `tools/extract_layout.py` writes with positions, names and codes only, no byte of the ROM; run it again when
  `symbols.txt`, `sound_symbols.txt`, `graphics_subjects.txt` or the extractor change. `tmnt_assets_check ROM_SET
  PYTHON_ASSETS [OUT]` (built with the reference tools) runs the first run into OUT and compares every file with
  the folder `extract_assets.py --out PYTHON_ASSETS` wrote.
- **Versions.** The first run writes `version.txt` last (the version of its tables and writers); a folder of
  another version is taken out again, and one with no `data/layout.txt` (a first run that did not finish) is not
  taken. `extract_assets.py`'s folders have no `version.txt` and are taken as they are.
- **The pack.** `tmnt.assets` is one file with what the game loads (the video and sound chips' memories and the
  programs' data, as the assets give them), for the devices, which never take the assets out of a ROM set:
  `"TMNT Arcade" --write-pack tmnt.assets` makes it from the assets. It holds the version of the tables the game's
  C reads by name: a pack of another build says so instead of loading.
- **The window** writes with Kenney Mini (CC0, `ports/sdl2/font/`, `font.bmp` beside the executable): none of the
  game's letters are there before its assets.

## The Windows package

What a player downloads: `tmnt-windows-x64.zip`, a folder `tmnt` with `TMNT Arcade.exe`, `TMNT.exe` (the console
version) and the `font.bmp` their first run and TMNT's menus write with, the package's `README.txt` (`docs/package/`: what the player needs, their own ROM set, how
the first run takes it, the controls), `LICENSE.txt`, and `licenses/` with MAME's and ymfm's terms (the board's
video and sound follow them), SDL2's and Kenney's. Nothing of the ROM set: the game takes its assets out of the
player's.

- **SDL2 inside.** `TMNT_STATIC_SDL2` builds SDL2 2.32.10 from its source (CMake's FetchContent, from GitHub) into
  the executable, and the C runtime with it: it needs no library beside it, nor Visual C++'s redistributable.
  Without it (the default) the build is the one above, SDL2's library beside the executable.
- **The icon** is the project's own drawing, nothing of the game's art (`tools/make_icon.py` writes
  `ports/sdl2/icon/`: a manhole's lid lit from below): a resource of the executable, which Explorer and the window
  show. A picture of your own takes its place in your builds: `local/icon_pc.png` (`local/` is never committed) or
  `-DTMNT_PC_ICON=PICTURE`.
- **Made locally** in a build folder of its own, without the reference tools, its executables in
  `build/release/bin` (the usual build's in `build/bin` stay as they are):

  ```bash
  cmake -S . -B build/release -DTMNT_STATIC_SDL2=ON -DTMNT_REFERENCE_TOOLS=OFF -DTMNT_BIN_DIR=build/release/bin
  ```

  ```bash
  cmake --build build/release --config Release --parallel 6 --target tmnt_package
  ```

  writes `build/package/tmnt-windows-x64.zip`, and the folder `build/package/tmnt` it packs.
- **The CI** (`.github/workflows/build.yml`) builds and packs it the same way on a clean Windows with no ROM set,
  on every push to main and its pull requests: the zip is the job's artifact, `tmnt-windows-x64`.

## The console version (TMNT)

`TMNT` (`build/bin/TMNT.exe`, beside `TMNT Arcade`) is the same game made for playing at home: the program with
the functions `console/replaced.txt` lists in the console's own versions (`console/*.c`; the decompiled C writes
their definitions as `CONSOLE_ORIGINAL(name)`, so TMNT Arcade runs the program as it is). It starts with a Konami
presentation of its own (the fixed layer's Konami logo, large, lit by the four turtles running across it), then a
menu under the title's logo: ARCADE (the arcade game with no coins: a player's START puts the coin and the attack
in, and the boxes say PRESS START), SPLINTER WAY (NORMAL, 5 lives and 3 continues; BOSS RUSH; RONIN), LAN (up to
four computers, `console/lan.c` over arcade-lan), OPTIONS (over BEST 10 PLAYERS' picture: CONTROLS, the cabinet's
DIP switches by the test mode's names, the region, VIDEO with the window, the pixels, WIDE 16:9 and 20:9 and FPS,
the volume, a sound test; kept in `tmnt.ini` beside the executable) and EXIT; the Konami code on the menu opens a
level select (any of the ten areas, infinite lives, every turtle in a red bandana, everyone the same turtle). A game
starts on the coin's screen, each player choosing the turtle he plays (the others come in with START).
Its menus write with the game's own letters; what it draws of the game's comes from your assets. Escape (or START of a
player in the game) pauses a game. `console/PLAN.md` has the plan, and what was done.

| Control | Keys |
|---|---|
| Move (player 1) | the arrow keys |
| Attack (in the menus: pick), jump (back) | Z or left Ctrl; X or left Alt |
| START | Enter, a controller's Start |
| Back in the menus | Escape |
| Full screen; the next kind of pixels | F11 or Alt+Enter; F3 |

`tmnt_console_headless` runs it with no window for scripted checks: the script's controls are the players',
`--actions FILE` gives the menus' actions by frame (`FRAME up|down|left|right|accept|back|start [PLAYER]`: with a
PLAYER, that player's own stick, buttons or START; `pause`, `quit`, `restart` in a game), `--wide 1|2` WIDE
(`--wide-check`: each frame, the columns of its sides that are not their map's), and
`--video-dump FRAME ... --video-dir DIR` writes those frames with the console's picture over them
(`tests/console/` has the menus' actions of a few runs):

```bash
build/bin/tmnt_console_headless --actions tests/console/arcade_donatello.txt --script tests/console/skip_intro.txt --video-dump 110 680 700 1200 --video-dir build
```
