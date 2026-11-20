TMNT Arcade and TMNT
====================

Konami's Teenage Mutant Ninja Turtles (1989), the arcade game for four players, as a native game:
decompiled to C and checked frame by frame against MAME, github.com/cheviman/tmnt-port. Nothing of the
game comes in this package: it takes the game's graphics and sounds out of your own ROM set.

Two games come in it, the same program:
- TMNT Arcade, the arcade game as it is: coins, the test mode, the four players.
- TMNT, the console version: the game made for playing at home, with menus of its own, no coins (START
  brings a player in), Splinter Way's modes and the game over the local network.

They are TMNT Arcade.exe and TMNT.exe on Windows, TMNT Arcade.app and TMNT.app on macOS, and TMNT Arcade
and TMNT on Linux and on a Raspberry Pi. On macOS each comes in a package of its own: tmnt-macos-universal.zip has TMNT (and
the tools for the devices), tmnt-arcade-macos-universal.zip TMNT Arcade.

What you need
-------------
tmnt.zip, MAME's ROM set of Teenage Mutant Ninja Turtles (World 4 Players, version X), as MAME has it.
Keep it beside the games, or drop it on the game's window when it asks for it; a folder rom beside the
games with the set's files (or tmnt.zip) in it works too. Keep the games' pictures (font.bmp,
input_prompts.bmp) beside them: the window that asks for the set and TMNT's menus write with them (on
macOS they are inside the .apps).

The first time a game starts it takes its assets out of the ROM set (a few seconds) and keeps them in
the folder assets: beside the games on Windows, in ~/.local/share/TMNT on Linux, in
~/Library/Application Support/TMNT on macOS. From then on both start from them and never open the ROM
set again. The assets come from your ROM set, so they are yours alone: never share them.

macOS: the .apps are not signed with a certificate. The first time, open one with a right click and
Open (on macOS 15 and later: System Settings, Privacy & Security, Open Anyway).
Linux: start ./TMNT or "./TMNT Arcade" from the folder; the games need the usual libraries of a
desktop (X11 or Wayland, ALSA, PulseAudio or PipeWire), which they load themselves.
Raspberry Pi (tmnt-raspberrypi-arm64.tar.gz): a Raspberry Pi 3 or later (3, Zero 2 W, 4, 400, 5, 500) with
the 64-bit Raspberry Pi OS, Debian 12 "Bookworm" or later; the 32-bit Raspberry Pi OS does not run it.
Start ./TMNT or "./TMNT Arcade" from the folder, as on Linux: in a terminal of the desktop, or with no
desktop at all (Raspberry Pi OS Lite), where the game takes the whole screen.

Playing TMNT Arcade
-------------------
The arcade game as it is: four players, each with a coin slot and no start button. A coin and the
attack button bring a turtle in: player 1 is Leonardo, 2 Michelangelo, 3 Donatello, 4 Raphael.

  Move (player 1)            the arrow keys
  Attack, jump               Z or left Ctrl; X or left Alt
  Both at once (special)     C
  Coins 1-4                  5, 6, 7, 8
  Service                    9
  Full screen                F11 or Alt+Enter
  Smooth or sharp pixels     F3
  Quit                       Escape

Game controllers are players 1 to 4 in the order they are plugged in: the pad or the left stick moves,
A attacks, B jumps, X does both at once, Back puts a coin in. A controller the games do not know, or know wrong, takes its
line (SDL_GameControllerDB's format) in gamecontrollerdb.txt, in the folder of the assets: the games
read it when they start, and its line goes over their own.

From a command line, TMNT Arcade with --service starts the game in its test mode, as the cabinet's
switch does.

Playing TMNT
------------
After the Konami presentation the arcade's attract mode plays (its story, the demos, the profiles), and
again after 30 seconds on the menu with nothing pressed: any key or button opens the menu.
The menu under the title's logo:
- ARCADE: the arcade game, its lives the cabinet's switches'. With OPTIONS > COINS on, with coins as the
  cabinet: keys 5, 6, 7, 8 (or a controller's Back) put in players 1-4's coins, and a coin picks a turtle,
  brings a player in or continues; START alone needs credits.
- SPLINTER WAY: NORMAL (5 lives and 3 continues), BOSS RUSH (from one boss to the next: each area
  crossed to its boss on its own; 5 lives and no continues) and RONIN (one player alone against the enemies
  of four).
- MULTIPLAYER: a game of up to four computers on the same network, one player each. HOST makes a room, JOIN
  finds the rooms and joins one, NAME is the name the room shows. The host's START begins the game on
  every computer, with the host's switches and picture. The first time, your system may ask whether
  TMNT may use the network: allow it on private networks.
- OPTIONS: the cabinet's switches, COINS, the region, the controls of each player, the picture (its size, the
  pixels, INTEGER SCALE: the pixels all the same size; WIDE: 16:10 as the Steam Deck's screen, 16:9 or 20:9;
  VSYNC, each frame at the screen's refresh; PRESS START under the boxes blinking, still or off), the
  volume, a sound test; kept in tmnt.ini.
- EXIT.
Choose a turtle, and the game starts with it. In a game, START brings in another player on a turtle
free; a player with no lives left comes back with START too (a continue), choosing his turtle again:
left and right, attack or START (jump: not now). Escape, or START of a player in the game, pauses it (CONTINUE,
RESTART, QUIT GAME). In a MULTIPLAYER game
there is no pause: Escape leaves the game, which goes on for the others.

  Move (player 1)            the arrow keys
  Attack, jump               Z or left Ctrl; X or left Alt
  Both at once (special)     C
  START                      Enter
  In the menus               the arrows move, attack picks, jump or Escape goes back
  Full screen                F11 or Alt+Enter
  The next kind of pixels    F3

Game controllers: the same, X both at once, Start is START, Back pauses (Start too, in the game). OPTIONS >
CONTROLS changes each player's keys and buttons, SPECIAL's among them.

The devices
-----------
The Nintendo Switch, Android (phones, tablets, TVs), iPhone and iPad, and Anbernic's H700 handhelds have
packages of their own in the release: tmnt-switch.zip, tmnt-android.zip, tmnt-ios.zip (the two games to
install yourself, its README says how) and tmnt-anbernic.zip. Each has the device's games
without assets (Android's, when the release signs them), ready to take them out of your tmnt.zip on the
device, and the tool that makes your own of them on a computer with your ROM set (for Windows, macOS and
Linux); its README says how.

Licenses
--------
LICENSE.txt is the project's (BSD-3-Clause). licenses/ holds MAME's and ymfm's (the board's video and
sound follow their behavior and tables), arcade-lan's (BSD-3-Clause: TMNT's MULTIPLAYER, its rooms and
datagrams, github.com/cheviman/arcade-lan), SDL's (zlib: the window, the sound and the controls, inside
the games), SDL_GameControllerDB's (zlib: the game controllers' mappings, inside the games) and Kenney's (CC0: the letters of the window that asks for the ROM set and of TMNT's menus,
and the pictures of the keys and buttons on its CONTROLS page).

The games are free, and stay free. To support the decompilations and the ports, and have a say in what
comes next: patreon.com/ChevimanPorts. Entirely optional; if anyone asks you to pay for these games, it
is not this project.

Teenage Mutant Ninja Turtles is a trademark of its owner, and the arcade game is Konami's. This project
is not affiliated with or endorsed by either, nor by Nintendo, Google, Apple, Anbernic or Raspberry Pi.
