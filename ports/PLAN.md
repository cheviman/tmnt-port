# TMNT Arcade on every platform: what it takes, and in what order

The arcade game as the decompilation has it (the same frame by frame as the reference: coins, the test mode, the
four players), on every platform the port has. segasonic-port and outrunners-port took only their console builds
to the devices; this one takes the arcade game there too. What they built is the model: read their folders.

## What the game has today

- `TMNT Arcade` (ports/sdl): the game in a window on SDL 3 (SDL 2 till 19-11-2026: the entries below say SDL2 where
  it was), the keyboard and four game controllers, built and
  played on Windows; `tmnt_game_headless` for the checks. Its Windows package (`tmnt_package`, the README's "The
  Windows package"): the executable alone with SDL2 inside it, an icon of our own, its README.txt and licenses, and
  the CI's windows job that makes it.
- `TMNT` (ports/sdl with the console, console/PLAN.md): the console version beside it, in the same package;
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
  settings and log in sdmc:/switch/tmnt (tmnt-arcade); its window the Switch's 1280 x 720, never full screen
  (devkitPro's SDL has its desktop mode 1920 x 1080 always, SWITCH_VideoInit: the picture was drawn for 1080p and the
  720p screen showed a part of it, issue 19), TMNT's WIDE 16:9; TMNT's menus with no EXIT (HOME), CONTROLS nor VIDEO.
  Built here with devkitPro (its MSYS2's aarch64-none-elf-cmake, with
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
  the menus' back and the pause, the background a pause, exit(0) at the end. ports/sdl/touch.c, TMNT's own: a stick
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

- **The touch controls' size** (07-11-2026: Sebas, on an open Fold "se va de tamaño"): ports/sdl/touch.c's unit was a
  tenth of the screen's height. Now (touch_layout, outrunners-port's rule): over the picture, a tenth of the area's
  smaller side (its height, or its width over 1.8: a phone's as before), at most 0.35 inch on the screen
  (SDL_GetDisplayDPI); on a screen taller than the picture by a fifth or more, the picture at the top and the
  controls along the middle of the room under it, START between the stick and the buttons, the unit the room's
  height over 4. Seen only as a drawing of the same formulas over the game's frames (local/touch_layout_mock.png),
  not yet on a Fold.

- **Game controllers' mappings** (07-11-2026: a player's Nvidia Shield, its PS4 and Xbox controllers moving no
  turtle and their A and B pausing): SDL_GameControllerDB (ports/sdl/controllerdb, zlib, its commit in its README),
  this platform's lines built into the games on the computers and Android (embed.cmake: a byte array, MSVC taking no
  string longer than 64 KB; CMakeLists.txt's tmnt_controller_db). The log says what SDL finds of each joystick (name,
  GUID, game controller or not, its mapping) and the first 48 keys and buttons pressed, for the player to send
  (Android: Android/data/<the app>/files/tmnt.log). 1.0.3 gave the mappings to SDL after SDL_Init
  (SDL_GameControllerAddMappingsFromRW), which in SDL 2.32 makes no game controller of a joystick already plugged in:
  SDL_PrivateAddMappingForGUID only remaps the controllers open, and SDL_GameControllerInit sends
  SDL_CONTROLLERDEVICEADDED only for the joysticks with a mapping then (the Shield's case: its controllers on before
  the game). Now (segasonic-port 1.4.0's way, main.c's add_mappings) one text, the built-in lines, the Anbernic's
  buttons and last the player's gamecontrollerdb.txt (its lines for this platform or for none, in the folder of what
  the game keeps or else beside the executable), is SDL_HINT_GAMECONTROLLERCONFIG before SDL_Init, which SDL reads
  before it looks for joysticks; a later line goes over an earlier one for the same GUID. Checked with SDL's dummy
  video: Windows "Game controllers: 885 mappings", 886 with a gamecontrollerdb.txt of one Windows line and one Linux
  line; Linux (WSL) 753, and 754 with that file beside the executable. Not yet on the Shield. An
  SDL_GAMECONTROLLERCONFIG in the environment (Steam sets one for the games it starts) went over the hint, SDL_GetHint
  taking it over a hint of normal priority: 304 mappings on Windows, the built-in lines and the file left out. Now its
  lines go into the text after the Anbernic's and before the file's, and the hint is SDL_HINT_OVERRIDE (C's getenv
  and SDL_SetHintWithPriority, which the Anbernic's stub has): 885, 886 with one line there, 887 with the file too
  (segasonic-port the same).

- **A Fold opened mid-game** (07-11-2026: Sebas, the picture in half the open screen): Android's WIDE was chosen once
  at the start. Now main.c's follow_screen (outrunners-port's) chooses it again whenever the renderer's size changes:
  20:9, 16:9 from 16:9 on and on a tablet's screen (its short side 4 inches or more, by the display's DPI: the open
  Fold, the picture at the top and the touch controls under it), 16:10 from 3:2 on, else none; not during a LAN game
  (the host's WIDE). touch.c reads the density again with the new size. Built for Android, not yet seen on a Fold.
- **The icon** (07-11-2026: Sebas): Raphael's face from a friend's mural, ports/sdl/icon/raphael_mural.png, of which
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
    (ports/sdl/icon/turtles_mural.png, tools/make_icon.py). TMNT APK takes a banner of the player's own: banner.jpg
    beside it, --banner, or a picture wider than tall dropped on its window (its row, TV banner).
  - **The TV's remote** (pad, OK and back alone): main.c's is_remote also knows a remote on Android by its buttons (no
    B and no Start), whatever its name. In TMNT its back is JUMP (the menus' back, the pause's CONTINUE) and not the
    pause, its OK attacks and is the START of a player out of the game (in, or a continue); home pauses
    (SDL_APP_WILLENTERBACKGROUND). SDL 2.32's Android joystick (the remote is one: SOURCE_DPAD) gives OK as A and back
    as Back. TMNT Arcade as before (back puts a coin in).
  Not yet run on a Chromecast: installing, the banner, the remote and the speed are to be seen there.

- **The first run on a Mac** (07-11-2026: Sebas's Mac, 1.0.5: "there is none beside it; the window that asks for
  one does not open", and nothing more). The .app downloaded and opened where it was unpacked runs from a copy macOS
  makes of it (App Translocation), alone in that copy's folder; the same with the .app alone moved to Applications.
  find_assets gave first_run the folder beside the .app as its base, so the window looked for its letters there and
  found no font.bmp. Now first_run takes the .app's Resources (SDL_GetBasePath) as its base, its letters there
  (outrunners-port's), and the ROM set there, beside the .app or in ~/Library/Application Support/TMNT. app_folder
  (first_run.c now, its raw NUL byte gone) gives the folder the player put a translocated .app in, from Security's
  SecTranslocateIsTranslocatedURL and SecTranslocateCreateOriginalPathForURL (dlsym, macOS 10.12 on), so tmnt.zip
  beside the .app is found there too. Checked: first_run.c built by zig cc for aarch64-macos and x86_64-macos (a
  stand-in CoreFoundation.h), Windows' first run from tmnt.zip beside the executable. Not yet on a Mac.

- **macOS's two packages** (09-11-2026, issue 7: Quiver Launcher, which installs games from GitHub's releases, took
  the macOS package's first .app, TMNT Arcade, and left TMNT out). The workflow's macOS job packs TMNT Arcade.app
  apart, with the README and the licenses: tmnt-macos-universal.zip has TMNT and the devices' tools,
  tmnt-arcade-macos-universal.zip TMNT Arcade; the release carries four packages. The other platforms as they were.
  Checked: the workflow's YAML read back (the artifact's two paths, the release's four files); the job itself runs at
  the next push.

- **Direct3D 11 on Windows** (09-11-2026, Sebas: "pone por default que use dx11", after a player asked whether the game
  was made with DX9): SDL 2's first renderer on Windows is Direct3D 9. main.c gives SDL_HINT_RENDER_DRIVER
  "direct3d11" before SDL starts (SDL tries the others where it cannot start; SDL_RENDER_DRIVER in the environment
  still chooses) and SDL_HINT_RENDER_BATCHING (a renderer named by the hint has it off); the log says "Renderer:". The
  picture is the same (the board's video is drawn by the CPU); Direct3D 11's swap chain is DXGI's flip model
  (SDL_render_d3d11.c: DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL), and Alt+Tab or the full screen lose none of its textures, as
  Direct3D 9's device did. Checked: TMNT and TMNT Arcade log direct3d11, and opengl with SDL_RENDER_DRIVER=opengl.

- **The devices' packages, without assets** (09-11-2026, Sebas: "hay 3 packs, win, linux, mac", "todos por
  separado sin assets"; as outrunners-port's of the same day, 5fd807b and 3687f84). The computers' packages hold the
  two games alone (tmnt_package, the CI's Pack steps); each computer's build leaves a kit (tmnt-OS-kit.tar.gz: TMNT
  NSP, TMNT APK, TMNT Anbernic, font.bmp, the licenses), and the job devices packs ports/pack_devices.py's
  tmnt-switch.zip, tmnt-android.zip and tmnt-anbernic.zip: each the device's games without assets (switch/, android/,
  anbernic/), the tool for Windows, macOS and Linux beside them ("TMNT NSP.exe", "TMNT NSP (macOS)", "TMNT NSP
  (Linux)"), font.bmp, the licenses and its README (docs/package/README-*.txt), the programs as Unix entries with
  their permissions; never a .nro, -debug.apk, .idsig or .assets of a computer's own. TMNT's own on top: the games
  also take the assets out of the player's tmnt.zip on the device (main.c's device_folder and device_assets,
  tmnt_extract linked into the three): sdmc:/switch/tmnt/ (both games), PORTS/tmnt/ (beside the executables),
  Android/data/<the app>/files/ (each app its own); tmnt.assets there, or the folder assets of this version, come
  first, and a pack the makers put in (the Switch's romfs, the APK, beside the handheld's executables) before them
  all. So the packages carry them ready too: the Switch's .nro without a pack in sd/switch/tmnt/ (switch.cmake makes
  them in build/bin/switch-nro where the build has no pack, the CI's case), Android's base APKs signed with the
  project's key in ready/ (the android job's Sign step, with the repository's secrets ANDROID_KEYSTORE, base64, and
  ANDROID_KEYSTORE_PASSWORD; without them the package has the tool's way alone, and its README leaves out the lines
  marked {ready}, those marked {no-ready} in their place; the key in local/android-release,
  ignored, its README says how to give it to GitHub), and anbernic/, which is the card's Roms/PORTS as it is. No ROM
  set on the device: the game says where to put it (the Switch's SDL has no message box: libnx's console till +; not
  the error applet, a fatal error out of an application from 10.0.0). Checked: a Linux handheld build in WSL
  (-DTMNT_HANDHELD, SDL's dummy video): no ROM set, the message with the folder; tmnt.zip there, the assets out in 4 s
  and the game on; started again and TMNT Arcade, from them. The Switch built with devkitPro without a pack
  (build/switch-release): TMNT.nro and TMNT Arcade.nro, 8 MB each. Anbernic with Zig (glibc 2.35, libSDL2-2.0.so.0 as
  before). Android: both base APKs signed with the new key, apksigner verify (v2, v3), no pack in them
  (assets/font.bmp alone), both ABIs. ports/pack_devices.py run here with the Windows tools and stand-ins for the
  others' (the three zips, the programs 755, this computer's -debug.apk and .idsig left out). rom_in_executables: the
  APKs, Anbernic's and the computers' 0; the .nro 3 + 3 + 28 stretches, which the Switch's ELF of 07-11 had already,
  before the extractor: 32 bytes of one 4-byte word over and over (02000000..., 0101 0000...), tables of the
  Switch's libraries that trivial() does not count as trivial (a byte or a 2-byte step, not a 4-byte period). The
  workflow itself runs at the next push; not tried on the devices themselves.
- **The testing pre-release** (09-11-2026, Sebas: "hagamos una rama de testing", "un release de testing"): the
  branch testing holds the next release's changes; each push to it builds as main does, and the workflow's job
  testing deletes the pre-release testing-build (its tag with it) and makes it again on that commit, the same seven
  packages, --prerelease and --latest=false (what follows the latest release, a launcher or the page's link, never
  takes it), docs/releases/testing.md and the commit as its notes. The tag is not named testing, which would make
  the branch's name ambiguous in git. TMNT's OPTIONS says which build: TMNT_VERSION_SUFFIX (-TEST<the run's number>,
  the workflow's env on testing, empty elsewhere) after PROJECT_VERSION in TMNT_VERSION (CMakeLists.txt); the
  Android versionCode, the Switch's and macOS's versions stay PROJECT_VERSION's. The way: changes to testing,
  pushed, the players try them; then testing merged into main and a release's tag, as before.

- **SDL 3 and the Raspberry Pi** (19-11-2026, Sebas: "primero pasarlo a sdl3 o el mas reciente que sirva con todo, y
  agrega una version de raspberi 3 en adelante. y va como regla para todos los ports"; and, asked about the Switch:
  "Probar SDL3 en Switch"). The window port is written for SDL 3 now (ports/sdl, which was ports/sdl2), one version
  on every platform that has it: SDL 3.4.20 (03-11-2026), CMakeLists.txt's TMNT_SDL_VERSION, its release's archive
  by its SHA-256 (TMNT_STATIC_SDL was TMNT_STATIC_SDL2).
  - **What each platform takes** (read on that day, not from memory). Windows, Linux, macOS and the Raspberry Pi:
    SDL 3.4.20 from its source, inside the games. Android: SDL 3.4.20 from its source, libSDL3.so beside libmain.so,
    its Java from its android-project (SDL 3.4 asks for API 21 and NDK r28c; its project compiles with SDK 35, what
    the build has). The Switch: SDL's own port is for those under Nintendo's NDA (its docs/README-switch.md) and
    devkitPro has no package of SDL 3 (its portlibs: switch-sdl2 2.28.5 alone), so switch.cmake builds SDL from
    devkitPro's own branch of it, github.com/devkitPro/SDL's switch-sdl-3.4 at c329016 (SDL 3.4.0 with libnx's video,
    sound and controllers; not released, its maintainer on its pull request 99: "a couple of tests failing"), by its
    archive's SHA-256, into the games; a devkitPro package (find_package(SDL3)) is taken first once there is one. The
    Anbernic: its stock firmware's own SDL 2.0.12, which no SDL 3 runs on (sdl2-compat is the other way round), so
    its build keeps SDL 2 under the same source: TMNT_SDL2_LAYER, ports/sdl/sdl2_layer.h, SDL 3's names the port
    uses over the SDL 2 functions that did the same (the events into a union of its own, the audio stream over the
    device's queue, float rectangles into whole ones, joysticks by the event's number), the stand-in library as
    before, and the build's last step ports/anbernic/check_sdl.py: the SDL functions the games' ELFs ask for, none
    newer than SDL 2.0.12 by SDL 2's headers.
  - **The move**: SDL's own rename scripts (its build-scripts) over the sources, then what changed underneath, by
    hand: its functions' bool results, the render calls' float rectangles, the window and the renderer made with
    no place and no flags (Direct3D 11 is SDL 3's first on Windows: the hint is gone), full screen as a bool, the
    events' fields and the window events as events of their own, joysticks by instance, the sound as a stream on the
    default device (SDL_OpenAudioDeviceStream, SDL_PutAudioStreamData; 1024 sample frames asked of the device, as
    before), SDL's base path and a dropped file's name SDL's own (not freed), the window's NSWindow and HWND from its
    properties, SDL_main.h included by main.c (the makers, with a main of their own, SDL_MAIN_HANDLED), textures'
    scale mode and blend mode said where SDL 2's defaults were counted on, Android's screen density from the
    display's content scale (SDL_GetDisplayDPI is gone: touch.c's unit as before, Android's densityDpi; main.c's
    tablet_screen by the density now, not the panel's own dots). port_sdl.h is what the port's files include: SDL 3,
    or the layer (not sdl.h, which a disk with no case would give for SDL 2's own SDL.h).
  - **Checked on Windows** (a worktree of its own, the SDL 2 build of the same commit kept beside it): the 18
    scenarios identical on both; TMNT Arcade's window run with attract's and one_player's scripts (--fast, --hashes,
    --video-hashes, --sound-hashes) the same on SDL 2, on SDL 3 and as the reference, frame by frame; the log "SDL
    3.4.20", "Renderer: direct3d11", "Game controllers: 887 mappings" (885 with SDL 2: SDL's own list), 888 with a
    gamecontrollerdb.txt of one Windows line and one Linux line, 889 with a line in SDL_GAMECONTROLLERCONFIG too, 888
    with that alone; the assets pack byte for byte the SDL 2 build's; tmnt_package's games with SDL and the C runtime
    inside (no SDL3.dll, no VCRUNTIME asked for), attract as the reference; TMNT's window seen (PrintWindow): its
    presentation and its menu, in a window and full screen (F11: 3840 x 2160, the picture 3027 wide in the middle),
    F3's pixels, the settings kept. rom_in_executables: the build's games 0; the package's two games 1 stretch of
    "tiles (planar)" and 1 of "sprites (planar)" each, which are SDL 3's own tables inside them (32-bit masks 1, 3,
    7 ... 255; 00 FF over and over), not the ROM set's (the same games with SDL beside them: 0): tools/rom_in_executables.py's
    trivial() does not know those two shapes yet (the decompilation's tool). Not played by hand yet: the sound heard,
    game controllers, the pause, VSYNC.
  - **What a player sees changed**: on Windows the window is DPI-aware (a 125% desktop: WINDOW 3 is its 942 x 672
    pixels, sharp, where Windows stretched SDL 2's to 1177 x 840); a Nintendo controller's buttons are by their place
    (SDL 3's SOUTH, EAST: no button labels), on a computer.
  - **Checked elsewhere, with no device**: Linux (WSL, SDL 3.4.20 inside; this WSL without XTest's and PipeWire's
    headers: SDL_X11_XTEST and SDL_PIPEWIRE off for the check, the CI installs them): its drivers X11, Wayland,
    KMS/DRM, ALSA, PulseAudio; TMNT Arcade's window with SDL's dummy video, attract and one_player as the reference
    in state and picture. The handheld's path (TMNT_HANDHELD, TMNT_HANDHELD_KEYS) built there twice by hand, with
    SDL 3 and through the layer with the system's SDL 2.30: both run attract as the reference, no warnings. The
    Anbernic's build (Zig, glibc 2.35): TMNT 72 SDL functions, TMNT Arcade 55, the newest of SDL 2.0.12. The
    Switch's (devkitPro's MSYS2 CMake on Windows writes SDL's precompiled header with a /d/ path its compiler does
    not find, and without that header SDL's Switch files compile to nothing: there SDL gets SDL_internal.h by the
    compiler's -include): TMNT.nro and TMNT Arcade.nro and the .nsps' pieces, "SDL-3.4.0" and its Switch video in
    them. macOS: main.c, controls.c and letters.c compiled by zig cc for arm64 and x86_64 (first_run.c's
    CoreFoundation needs a Mac: its SDL part is the Linux one). Android (NDK 28.2, SDK 35): both games' base APKs and this computer's signed ones, apksigner verifies them (v2, v3), aligned;
    aapt: io.github.cheviman.tmnt and .tmnt.arcade, SDK 24 to 35, the leanback activity and the banner, arm64-v8a and armeabi-v7a, each with
    libmain.so and libSDL3.so; rom_in_executables 0 on them.
  - **Not tried on a device**: the Switch (the screen: this SDL has the mode 1280 x 720 handheld and 1920 x 1080
    docked and resizes the window itself, issue 19's ground; the sound; the controllers), a phone or a TV, the
    handheld, a Mac.
  - **The Raspberry Pi** (3 and later; ports/raspberrypi). A computer: the Linux games for 64-bit ARM, the package
    tmnt-raspberrypi-arm64.tar.gz (the two games, their pictures, README.txt, the licenses). 64-bit: every model
    from the 3 on runs the 64-bit Raspberry Pi OS (3B, 3B+, 3A+, Zero 2 W, 4, 400, 5, 500, the Compute Modules:
    raspberrypi.com/software/operating-systems), and aarch64 is the build the Switch, Android and the Anbernic have;
    no 32-bit package. Built on Debian 12 "Bookworm" for arm64 (Raspberry Pi OS Legacy's base, glibc 2.36), so the
    games run on it and on Debian 13's (Trixie, glibc 2.41), SDL 3 inside them (Bookworm has none, Trixie 3.2.10)
    with Wayland, X11, KMS/DRM (no desktop: Raspberry Pi OS Lite), ALSA, PulseAudio and PipeWire. build.sh builds and
    packs; the CI's raspberrypi job runs it in debian:bookworm on an arm64 runner; on a PC wsl.sh makes that system
    in a folder of WSL's and runs it under QEMU. Checked here under QEMU, in that
    system: the games ask for libc and libm alone (glibc 2.34 at most) and for no SDL function of a library; SDL's drivers there X11, Wayland,
    KMS/DRM, ALSA, PulseAudio, PipeWire; the package's TMNT Arcade takes the assets out of tmnt.zip beside it (the first run on arm64) and
    plays attract's 7201 frames as the reference in state, picture and sound, with SDL's dummy video ("SDL 3.4.20", 767 mappings); TMNT runs a
    script and leaves; the decompilation's scenarios on its headless game: 17 identical with the check (16 at a time, area2 again two at a
    time: QEMU takes the long ones past the check's 15 minutes), and area4, its 81,601 frames run alone with no limit, identical in state, picture and sound (14 minutes).
    Not tried on a Raspberry Pi: the screen (Wayland, X11, KMS/DRM), the sound, game controllers, the speed on a Pi 3.

- **iOS and iPadOS** (20-11-2026; Sebas, 19-11-2026: "y anota que hay que agregar una version de ios", and, asked
  whether in this batch: "si, al final"; a platform of every port from here on). simpsons-port's iOS version (its
  ports/PLAN.md, 13-11-2026: Sebas's choices there), on SDL 3, which has iOS (SDL 3.4.20: iOS 11 on, Xcode 12.2 on;
  its docs/README-ios.md): the move to SDL 3 leaves it in. TMNT and TMNT Arcade for an iPhone or an iPad (arm64, iOS
  13 on), never the App Store (the game is Konami's, the player brings the ROM set): an unsigned .ipa of each in
  tmnt-ios.zip, which the player's sideloading tool signs as it installs it (AltStore, SideStore, Sideloadly:
  docs/package/README-ios.txt).
  - **What it is**: ports/ios (ios.cmake: CMake's Xcode generator, SDL 3.4.20 static, no HIDAPI, target
    tmnt_ios_games; Info.plist.in; LaunchScreen.storyboard, black; Assets.xcassets, Raphael's face 1024 x 1024 from
    tools/make_icon.py), CMakeLists.txt's iOS lines (TMNT_IOS and TMNT_DEVICE where CMAKE_SYSTEM_NAME is iOS,
    SDL_GameControllerDB's 42 iOS lines, arcade-lan's ARCADE_LAN_NO_BROADCAST), and main.c's SDL_PLATFORM_IOS branches,
    Android's build with Apple's own: the whole screen at its pixels (full screen, borderless, high pixel density),
    landscape, the home indicator dimmed, the sound as AVAudioSession's playback, the screen kept on, the picture and
    TMNT's WIDE between the sides the notch or the Dynamic Island takes (ios_side_inset: SDL's safe area of the
    window), nothing drawn from the background (an event watch) and a game paused there, the ROM set, the assets, the
    settings and the log in the app's Documents (the Files app's "On My iPhone" > the game), the touch controls' size
    by the screen's points (touch.c: 163 an inch on an iPhone, 132 on an iPad, times the pixels a point is). The CI's
    jobs ios (macos-15: the apps and their .ipa, read back: the Info.plist's keys, arm64 for iOS, the launch screen
    and the icon in the bundle, no assets in the .ipa) and ios-simulator (both apps started in the Simulator with no
    ROM set: each must write its log and say where to put tmnt.zip); ports/pack_devices.py --ios, tmnt-ios.zip; the
    release's nine packages.
  - **Checked here, with no Mac**: main.c, controls.c, letters.c and touch.c of both games compiled by Zig 0.17 for
    aarch64-ios and aarch64-ios-simulator with its Darwin headers and SDL 3.4.20's (-Wall -Wextra): 14 compiles, no
    warning, Mach-O arm64 for iOS (platform 2) and its Simulator (7), main renamed SDL_main, the iOS branches in
    them; the whole game for aarch64-ios: runtime/src, src/, console/ and arcade-lan's sources, 85 files, no error, and rom_in_executables 0 over all those objects; ios.cmake read by CMake with stand-in commands (an iPhone:
    TMNT.ipa and TMNT Arcade.ipa; the Simulator: the apps alone); the workflow's YAML parsed, its ten jobs;
    pack_devices.py --ios with stand-in .ipa files (tmnt-ios.zip: 11 files). The other platforms built again after
    it, as before.
  - **Not built, not run**: Xcode's build itself (the storyboard, the asset catalog, SDL's CMake under the Xcode
    generator, the link) is the CI's ios job's, at the next push, or a Mac's; nothing tried in the Simulator nor on an
    iPhone or iPad. To see there first: the game's tasks. runtime/src/tasks.c (the decompilation's) switches them with
    ucontext on Apple's systems, which macOS has and iOS may not give (its getcontext may answer "not supported":
    the log would say "A task could not be created"); if so, the way is tasks.c's own aarch64 switch, as on the
    Switch and Android, written for Mach-O, a change of the decompilation's. And MULTIPLAYER: iOS gives an app no
    broadcast without Apple's entitlement, and the arcade-lan this repository has (a7a1548) has no
    ARCADE_LAN_NO_BROADCAST yet (tmnt-port's newer one has).
- **The video's speed** (20-11-2026; Sebas, after simpsons-port's players' issue 21, the Switch at 37 to 50 pictures a
  second with its sound breaking up: how that stands in TMNT, the same family of board; and "acordate, nada de
  frameskip": no frame is left undrawn, ports/sdl/main.c's pace is as it was). The runtime's video
  (runtime/src/video.c: the decompilation's, changed there without WIDE and brought with a merge, the port's WIDE in
  it here) drew each pixel of the three layers on its own, its tile looked up in the tile RAM every time. Now a
  tile's row of 8 pixels at a time and a sprite cell's row of 16 (four bytes of the ROM, a bit plane each, through a
  table of 256 into the pens in one number); a layer with one scroll a tile at a time (every layer of stage 1's
  play: a tile's 8 rows are together in the ROM, a line's 40 tiles are not), an empty tile left out at once, a whole
  row with no pen 0 written as 8 pixels; the other scroll kinds a line at a time, each cell looked up once for the
  lines that cross it; the palette's colours worked out again only where the palette RAM changed; the pens into
  colours a line at a time, and WIDE's 320 board columns copied out of its frame instead of made twice. What
  tmnt_video_tiles_shown marks (the reference's --graphics-uses) goes a line at a time, so that each code's palette
  is its last pixel's as before. Nothing kept between frames but the palette's colours, which make themselves again
  from the palette RAM.
  - **Measured** (this computer, a Ryzen 9 5900XT with nothing else running; the first numbers, taken while other
    work ran on it, were three times these and are not kept. MSVC Release, stage 1's play, 6000 frames, timers
    round the video's parts in a copy of it outside the repository, the least of 3 runs; microseconds a frame
    drawn, before -> after):
    - 4:3: layer B 585 -> 48, layer A 576 -> 33, the fixed layer 557 -> 12, the sprites 135 -> 33, the pens into
      colours 31 -> 22, the palette 4 -> 1; the video 1889 -> 149, 12.7 times less.
    - WIDE 16:10: 689 -> 57, 674 -> 38, 627 -> 13, 123 -> 31, 89 -> 30, 4 -> 1; 2205 -> 170, 13.0 times.
    - WIDE 16:9: 747 -> 61, 733 -> 41, 672 -> 14, 121 -> 30, 94 -> 31, 4 -> 1; 2371 -> 177, 13.4 times.
    - WIDE 20:9: 831 -> 68, 815 -> 44, 735 -> 14, 102 -> 24, 101 -> 33, 4 -> 1; 2588 -> 184, 14.1 times.
    - GCC 13.3 (WSL), the Switch's kind of compiler: 4:3 1738 -> 136, WIDE 16:9 2214 -> 164.
    - The whole frame in the headless game: 0.45 ms not drawn (the program and the sound board); drawn and hashed
      2.48 -> 0.79 ms in 4:3, 3.04 -> 0.83 ms in WIDE 16:9 (the hash is 0.19 ms of each).
  - **The Switch: an estimate, not a measure.** simpsons-port's case put a core of the Switch's at some 6 to 8 times
    this computer's time. By that, a frame of the game with its picture in 16:9, the Switch's (0.45 + 2.2 ms here),
    took 16 to 21 ms there against the board's 16.9: at its limit or past it, and past it the game and its sound
    slow down together; now (0.45 + 0.17 ms) some 4 to 5 ms. The window's own work a frame (the picture's copy, the
    console's picture over it, the texture, the screen's scaling) is in neither number: not measured.
  - **Checked.** The 18 scenarios identical in state, picture and sound (check_scenarios --native-only --video
    --sound), here and in the decompilation with its own video. Every frame's hash of WIDE's picture and of the
    board's (tmnt_console_headless --wide-hashes FILE, new: docs/port.md), tests/console's nine runs and the 18
    scenarios with --arcade-start, each in WIDE OFF, 16:10, 16:9 and 20:9, the video before against the one after:
    216 files, 4,706,264 hashes, none different. A differential check outside the repository (build/perf_work/fuzz:
    the video before, its public names renamed, and the new one in one executable, drawing the same random boards:
    ROMs with empty, whole and half empty tiles and cells, the 64 scroll controls, the screen and the tiles
    flipped, the ROM banks, sprites of every size with flips and shadows, the 051937's registers, WIDE's sides,
    layer windows and fixed band in and out of range, frames not drawn, the tables of what was shown): 8 seeds of
    3000 frames identical, the port's video and the decompilation's; and to see that it looks where it must, 21
    bugs put into the old video on purpose and 30 into the new one, one at a time: all caught. rom_in_executables 0
    in TMNT.exe, TMNT Arcade.exe and the two headless games. Built: Windows (MSVC, no warnings), Linux (WSL, GCC
    13.3, SDL 3.4.20 inside; attract, one_player and area6 in WIDE 16:9 with the hashes of Windows' build before),
    the Anbernic (Zig: TMNT 72 SDL functions, TMNT Arcade 55, the newest of SDL 2.0.12), Android (NDK 28.2, both
    ABIs, the APKs), the Switch (devkitPro: TMNT.nro and TMNT Arcade.nro). video.c with GCC's and Clang's -Wall
    -Wextra: no warnings.
  - **Not tried, not done.** No device: the speed on a Switch is the players' to tell (testing's list asks for
    it). macOS, iOS and the Raspberry Pi are the CI's builds. The window's own work a frame was not measured; if a
    device still falls short, that and the program's 0.45 ms are what is left to look at, the video being a
    quarter of a frame's time now.
