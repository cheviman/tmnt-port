# TMNT Arcade on every platform: what it takes, and in what order

The arcade game as the decompilation has it (the same frame by frame as the reference: coins, the test mode, the
four players), on every platform the port has. segasonic-port and outrunners-port took only their console builds
to the devices; this one takes the arcade game there too. What they built is the model: read their folders.

## What the game has today

- `TMNT Arcade` (ports/sdl2): the game in a window on SDL2, the keyboard and four game controllers, built and
  played on Windows; `tmnt_game_headless` for the checks. Its Windows package (`tmnt_package`, the README's "The
  Windows package"): the executable alone with SDL2 inside it, an icon of our own, its README.txt and licenses, and
  the CI's windows job that makes it.
- `TMNT` (ports/sdl2 with the console, console/PLAN.md): the console version beside it, in the same package;
  `tmnt_console_headless` for its scripted checks.
- The assets come from `tools/extract_assets.py` (Python) and the game loads the folder (`runtime/src/assets.c`,
  `music.c`). No executable holds a byte of the ROM set (`tools/rom_in_executables.py`).

## The order

Sebas, 04-11-2026: "primero una version completa y dps port": the whole game first on Windows (the arcade game and
Splinter Way, console/PLAN.md, complete), then the other platforms.

1. **The first run in C.** The game takes the player's ROM set itself, as the other ports do: a C extractor
   (`runtime/src/extract.c`, its layout generated from what `extract_assets.py` writes) makes the same `assets/`
   folder the Python one does, byte for byte (a check that compares the two), and a pack of the loaded assets
   (`tmnt.assets`) for the devices, which never extract. On a computer, with no assets: a window that asks for the
   ROM set (dropped on it, or beside the executable) and says what it is doing and what failed.
2. **The computers' packages, Windows first** (Sebas, 04-11-2026: "hagamos primero el win"): SDL2 static, an icon
   of our own (nothing of the game's art), the package (the game, its README.txt and licenses); then Linux and
   macOS (the .app), the CI (`.github/workflows/build.yml`) building and packing the three, and the release on a
   tag. Every device has its maker, a tool the player runs on a computer with the ROM set, as the other ports
   have: the NSP's (Switch), the APK's (Android), the Anbernic's.
3. **Nintendo Switch.** Four players: each Joy-Con or controller a player (the cabinet's four slots). The NSP
   made on the player's PC with the player's ROM set (the pack inside it, never shared), as outrunners-port's
   tool does.
4. **Android.** The APK made on the player's PC, the same way; touch controls for one player (a stick, attack,
   jump, the coin), and game controllers for the four.
5. **Anbernic** (H700, stock firmware): cross-compiled, the pack beside the executable.

## To check on the way

- `runtime/src/tasks.c` has no aarch64 path for the Switch and Android (outrunners-port's keeps the registers on
  each task's stack): whether the decompiled game still needs it, or only the translation scaffold does.
- `local/` (the player's own pictures for the icons) goes into `.gitignore`.

## What was done (06-11-2026: Sebas, "hagamos las plataformas")

How segasonic-port and outrunners-port build each platform was read first (outrunners-port's is the model: one
SDL2 main for every platform with #ifdefs and feature macros, a .cmake of each device, makers in C on the computers
for the NSP, the APK and the Anbernic's PORTS, the aarch64 tasks for the Switch and Android).

- **Linux.** Built in WSL's Ubuntu 24.04 (gcc 13, CMake 3.28, SDL2 2.30; `-DTMNT_BIN_DIR=build/bin-linux`, so that
  its executables are not beside the Windows ones, which tools/core/check_scenarios.py would run first): the 18
  scenarios identical with frames, picture and sound (`--bin build/bin-linux --out build/scenarios_frames`), and
  TMNT's tests the same frame by frame as on Windows. gcc found what MSVC let by: the console's static game_over over
  the game's own (slots_game_over now). The icon's .png beside the executables.
- **macOS.** Both games as .apps (MACOSX_BUNDLE, io.github.cheviman.tmnt-arcade and .tmnt, the project's version),
  their pictures and tmnt.icns in Resources; the assets and the ROM set looked for beside the .app too (app_folder,
  outrunners-port's: SDL_GetBasePath is the .app's Resources). tasks.c's ucontext there already had _XOPEN_SOURCE.
- **The CI**: linux-x64 (ubuntu-24.04, SDL2's build headers), macos-universal (macos-15, arm64 and x86_64, macOS 11
  on, the .apps signed ad hoc) beside windows-x64; Linux's package a .tar.gz (the artifacts' zip loses the executable
  bits), macOS's a ditto .zip; on a tag v* the release with the three and docs/releases/<tag>.md as its notes.
  docs/package/README.txt for the three systems (where the assets go, macOS's Open Anyway) and TMNT's menu as it is.

- **The Switch.** The game's tasks on aarch64 (the decompilation's tasks.c, 9d43e63: x19-x30 and d8-d15 on each task's
  stack, outrunners-port's; newlib and bionic have no ucontext). ports/switch/switch.cmake: TMNT and TMNT Arcade, two
  titles (0500544d4e540000, 0500544d4e541000: homebrew's range), each the window port's main.c with TMNT_HANDHELD,
  TMNT_SCREEN_WIDE and TMNT_SYSTEM_EXIT, its .nsp's pieces in build/bin/switch/tmnt and tmnt-arcade (main, main.npdm
  from ports/switch/npdm.json.in, control.nacp, icon.jpg drawn from the project's icon) and, where the build has the
  pack, an .nro of each. On a device main.c reads the pack (romfs:/tmnt.assets) and never runs the first run; its
  settings and log in sdmc:/switch/tmnt (tmnt-arcade); its window the Switch's 1280 x 720, TMNT's WIDE 16:9; TMNT's
  menus with no EXIT (HOME), CONTROLS nor VIDEO. Built here with devkitPro (its MSYS2's aarch64-none-elf-cmake, with
  CMAKE_DEPENDS_USE_COMPILER=FALSE: its gcc writes D:/ paths that make cannot read again). TMNT NSP
  (ports/switch/nsp, outrunners-port's, the game's own extraction and pack: rom_pack.c) makes both .nsps, TMNT
  Arcade.nsp beside TMNT.nsp; tried with keys made up: tools/check_nsp.py finds every hash holding, the program and
  control data the build's, the pack identical to the first run's. The CI's switch job (devkitpro/devkita64) and its
  pieces in every package beside TMNT NSP (the Windows one by tmnt_package: an .nro never goes in a package).
  Not yet run on a Switch.

To come, in this order: Android (android.cmake, the touch controls, the APK's maker), the Anbernic (Zig's aarch64
toolchain, the SDL2 stub, its maker).
