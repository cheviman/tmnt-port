TMNT Arcade and TMNT for Android
================================

Konami's Teenage Mutant Ninja Turtles (1989), the arcade game for four players, as a native game:
decompiled to C and checked frame by frame against MAME, github.com/cheviman/tmnt-port. TMNT Arcade is
the arcade game as it is; TMNT, the console version (menus of its own, no coins, Splinter Way's modes,
the game over the local network). This is Android's package:
{ready}the games without their assets, ready to install, and the tool that makes your own APKs of them on
{ready}your computer.
{no-ready}the tool that makes your own APKs of the games on your computer.
Nothing of the game comes in this package: the games take their graphics and sounds out of your own ROM set.

What you need
-------------
- tmnt.zip, MAME's ROM set of Teenage Mutant Ninja Turtles (World 4 Players, version X), as MAME has it;
- a phone, tablet or TV with Android 7 or later.

{ready}The APKs in ready/
{ready}------------------
{ready}Install ready/TMNT.apk and ready/TMNT Arcade.apk on the device (open them there; Android asks once to allow
{ready}apps from that source). Open a game once: it says it has no ROM set and makes its folder on the device's
{ready}storage, Android/data/io.github.cheviman.tmnt/files (TMNT Arcade's:
{ready}Android/data/io.github.cheviman.tmnt.arcade/files). Copy tmnt.zip into that folder, from a computer with the
{ready}device on USB (file transfer: most phones' own file managers do not open Android/data), and open the game
{ready}again: the first start takes the assets out of tmnt.zip there (some seconds). Each game its own copy. These
{ready}APKs are signed with the project's key: a new release installs over them, but not over an APK you made with
{ready}the tool (another key: uninstall that one first).
{ready}
Your own APKs, with the tool
----------------------------
Start the tool for your computer, with a double click:

  Windows   TMNT APK.exe
  macOS     TMNT APK (macOS)    (the first time: right click, Open)
  Linux     TMNT APK (Linux)

Leave it where it is, beside the folder android (the games' Android build) and font.bmp. Drop tmnt.zip on
its window (or put it beside the tool), a square picture of yours for the launcher's icon and a 16:9 one
for a TV's banner if you want them. It writes TMNT.apk and TMNT Arcade.apk with the assets in them, signed
with a key of yours kept in your user folder (keep it: only an APK with the same key installs over the
last one). Copy them to the device and open them there.

{ready}What either way makes from your ROM set is yours alone: never share it.
{no-ready}What the tool makes from your ROM set is yours alone: never share it.

Playing
-------
The touch screen has a stick where your left thumb lands, ATTACK and JUMP at the right, and START (COIN on
TMNT Arcade); a game controller hides them, and each controller is a player. Back, or START in the game,
pauses TMNT; OPTIONS > VIDEO has the pixels, INTEGER SCALE, PRESS START and FPS. On a TV with Android TV or
Google TV the games are in the TV's apps; its remote is a player after the game controllers (in TMNT its pad
moves, OK attacks, back jumps, home pauses). Each game writes its log, tmnt.log, in its folder on the device's
storage, Android/data/io.github.cheviman.tmnt/files (TMNT Arcade's:
Android/data/io.github.cheviman.tmnt.arcade/files).

Licenses
--------
LICENSE.txt is the project's (BSD-3-Clause). licenses/ holds MAME's and ymfm's (the board's video and
sound follow their behavior and tables), arcade-lan's (BSD-3-Clause: TMNT's MULTIPLAYER), SDL2's and
SDL_GameControllerDB's (zlib) and Kenney's (CC0: the letters, the controls' pictures).

Teenage Mutant Ninja Turtles is a trademark of its owner, and the arcade game is Konami's; Android is a
trademark of Google. This project is not affiliated with or endorsed by them.
