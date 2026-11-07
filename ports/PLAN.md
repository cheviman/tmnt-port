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

- **Android.** ports/android/android.cmake (the NDK's toolchain, no Gradle: javac, d8, aapt2, aapt, zipalign,
  apksigner): TMNT and TMNT Arcade, each its libmain.so (SDL2 shared, from its source) with TMNT_HANDHELD,
  TMNT_SYSTEM_EXIT and TMNT_TOUCH (TMNT's WIDE as the phone's screen is: 20:9, 16:9, or none squarer than 3:2), its
  manifest from AndroidManifest.xml.in (io.github.cheviman.tmnt and .tmnt.arcade), the launcher's icon drawn from the
  project's (ports/android/res); of each a base APK (no pack, no signature) and this computer's -debug.apk (with
  build/bin/tmnt.assets, the debug key). main.c on Android: the pack read whole from the APK's assets
  (assets_load_pack_bytes, runtime/src/assets_pack.c: the port's), the log in the app's folder on the storage, back
  the menus' back and the pause, the background a pause, exit(0) at the end. ports/sdl2/touch.c, TMNT's own: a stick
  where the left thumb lands (eight ways), ATTACK and JUMP at the right (a thumb sliding between them), START (COIN in
  TMNT Arcade) at the top right, player 1's; a game controller or a key hides them. Built here with segasonic-port's
  downloaded SDK copied into build/downloads/android (NDK 28.2, build-tools 35.0.1, JDK 17; Ninja from Visual
  Studio): apksigner verifies them (v2, v3), aligned, their packages and names. TMNT APK (ports/android/apk,
  outrunners-port's maker) makes both, TMNT Arcade.apk beside TMNT.apk, signed with the player's key (made once in
  %APPDATA%\TMNT): apksigner verifies them, the pack the first run's. The CI's android job and its base APKs in every
  package beside TMNT APK. Not yet run on a phone (nor the touch controls seen).

- **Anbernic** (H700, the stock firmware). ports/anbernic/anbernic.cmake with zig_aarch64.cmake (outrunners-port's:
  Zig 0.17's C compiler for aarch64-linux-gnu.2.35, the firmware's Ubuntu 22.04), TMNT and TMNT Arcade with
  TMNT_HANDHELD and TMNT_HANDHELD_KEYS, linked against a stand-in of the firmware's SDL2 (sdl2_stub.c: the functions
  main.c calls, by name, as libSDL2-2.0.so.0) with SDL 2.32's headers, so the handheld's own SDL 2.0.12 runs them.
  main.c on it: the handheld's buttons as a game controller's (the mapping as a hint before SDL starts), MENU (button 8)
  leaves the game, the volume keys OPTIONS > VOLUME's, the whole screen (640 x 480, the game's 4:3), its settings
  beside the game on the card. In build/bin/anbernic, PORTS's files without the pack: TMNT.sh and TMNT Arcade.sh, the
  folder tmnt with both executables and input_prompts.bmp. Checked here: ELF aarch64, glibc 2.34 at most, NEEDED
  libSDL2-2.0.so.0 and the C library alone, no SDL function newer than 2.0.12. TMNT Anbernic (ports/anbernic/maker,
  outrunners-port's) writes PORTS from them and the player's ROM set, the pack identical to the first run's. The CI's
  anbernic job and its files in every package beside TMNT Anbernic; arcade-lan's license in the packages too. Not yet
  run on a handheld.

- **The touch controls' size** (07-11-2026: Sebas, on an open Fold "se va de tamaño"): ports/sdl2/touch.c's unit was a
  tenth of the screen's height. Now (touch_layout, outrunners-port's rule): over the picture, a tenth of the area's
  smaller side (its height, or its width over 1.8: a phone's as before), at most 0.35 inch on the screen
  (SDL_GetDisplayDPI); on a screen taller than the picture by a fifth or more, the picture at the top and the
  controls along the middle of the room under it, START between the stick and the buttons, the unit the room's
  height over 4. Seen only as a drawing of the same formulas over the game's frames (local/touch_layout_mock.png),
  not yet on a Fold.

- **Game controllers' mappings** (07-11-2026: a player's Nvidia Shield, its PS4 and Xbox controllers moving no
  turtle and their A and B pausing): SDL_GameControllerDB (ports/sdl2/controllerdb, zlib, its commit in its README),
  this platform's lines built into the games on the computers and Android (embed.cmake: a byte array, MSVC taking no
  string longer than 64 KB; CMakeLists.txt's tmnt_controller_db), given to SDL after SDL_Init. The log says what SDL
  finds of each joystick (name, GUID, game controller or not, its mapping) and the first 48 keys and buttons pressed,
  for the player to send (Android: Android/data/<the app>/files/tmnt.log). Checked: the Linux build in WSL with SDL's
  dummy video, "Game controllers: 453 mappings of SDL_GameControllerDB" (745 for Linux, the rest SDL's own updated).
  Not yet on the Shield.

- **A Fold opened mid-game** (07-11-2026: Sebas, the picture in half the open screen): Android's WIDE was chosen once
  at the start. Now main.c's follow_screen (outrunners-port's) chooses it again whenever the renderer's size changes:
  20:9, 16:9 from 16:9 on and on a tablet's screen (its short side 4 inches or more, by the display's DPI: the open
  Fold, the picture at the top and the touch controls under it), 16:10 from 3:2 on, else none; not during a LAN game
  (the host's WIDE). touch.c reads the density again with the new size. Built for Android, not yet seen on a Fold.
- **The icon** (07-11-2026: Sebas): Raphael's face from a friend's mural, ports/sdl2/icon/raphael_mural.png, of which
  tools/make_icon.py makes every size (the computers', the Android launcher's, the Switch's); the drawn manhole gone.

- **The Chromecast with Google TV** (07-11-2026: Sebas, "un port especial para el chromecast de 4ta generacion", for his
  nephews). Its Android is 32-bit (armeabi-v7a alone) on a 64-bit chip, and Google TV shows only the apps that ask for
  its launcher. Not a build of its own: the same APKs, on phones and TVs alike.
  - **32-bit ARM**: runtime/src/tasks.c (the decompilation's, D:/vs-cheviman/tmnt first) switches the game's tasks on
    32-bit ARM as on aarch64: r4-r12, lr and d8-d15 on the task's stack, a naked function in unified syntax (the same
    in ARM and Thumb). android.cmake builds armeabi-v7a's libraries too, a build of its own of the same source
    (ExternalProject in build/android/abi/armeabi-v7a, TMNT_ANDROID_LIBRARIES_ONLY), and each APK has both ABIs:
    Android installs the one its device runs. No change in the CI's job.
  - **The check without the device**: ports/android/zig_arm_check.cmake builds the headless games for 32-bit ARM
    Linux (Zig, musl, Thumb-2 with VFPv3-D16 as armeabi-v7a), and WSL's qemu-arm runs the scenarios: 4-byte pointers,
    char without a sign, the tasks, and the alignment QEMU holds LDRD/LDM/VLDM to (an unaligned one ends a 32-bit
    process on Android's arm64 kernels). The 18 scenarios identical frame by frame, state, picture and sound
    (build/scenarios_arm32; area2, area4 and area5 run again three at a time and with no time limit: qemu takes them
    past check_scenarios.py's 15 minutes six at a time). The NDK's armeabi-v7a library switches tasks with the same
    Thumb-2 instructions the check ran.
  - **Google TV's launcher**: the manifest's LEANBACK_LAUNCHER and android.software.leanback (not required), and a
    banner, res/drawable-xhdpi/banner.png (320 x 180): the four turtles' faces from the same friend's mural
    (ports/sdl2/icon/turtles_mural.png, tools/make_icon.py). TMNT APK takes a banner of the player's own: banner.jpg
    beside it, --banner, or a picture wider than tall dropped on its window (its row, TV banner).
  - **The TV's remote** (pad, OK and back alone): main.c's is_remote also knows a remote on Android by its buttons (no
    B and no Start), whatever its name. In TMNT its back is JUMP (the menus' back, the pause's CONTINUE) and not the
    pause, its OK attacks and is the START of a player out of the game (in, or a continue); home pauses
    (SDL_APP_WILLENTERBACKGROUND). SDL 2.32's Android joystick (the remote is one: SOURCE_DPAD) gives OK as A and back
    as Back. TMNT Arcade as before (back puts a coin in).
  Not yet run on a Chromecast: installing, the banner, the remote and the speed are to be seen there.
