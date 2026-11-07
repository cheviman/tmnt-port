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
and TMNT on Linux.

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
Linux: start ./TMNT or "./TMNT Arcade" from the folder; the games need SDL2's usual libraries of a
desktop (X11 or Wayland, ALSA or PulseAudio), which they load themselves.

Playing TMNT Arcade
-------------------
The arcade game as it is: four players, each with a coin slot and no start button. A coin and the
attack button bring a turtle in: player 1 is Leonardo, 2 Michelangelo, 3 Donatello, 4 Raphael.

  Move (player 1)            the arrow keys
  Attack, jump               Z or left Ctrl; X or left Alt
  Coins 1-4                  5, 6, 7, 8
  Service                    9
  Full screen                F11 or Alt+Enter
  Smooth or sharp pixels     F3
  Quit                       Escape

Game controllers are players 1 to 4 in the order they are plugged in: the pad or the left stick moves,
A attacks, B jumps, Back puts a coin in.

From a command line, TMNT Arcade with --service starts the game in its test mode, as the cabinet's
switch does.

Playing TMNT
------------
The menu under the title's logo:
- ARCADE: the arcade game, its lives the cabinet's switches'.
- SPLINTER WAY: NORMAL (5 lives and 3 continues), BOSS RUSH (from one boss to the next: each area
  crossed to its boss on its own) and RONIN (one player alone against the enemies of four).
- MULTIPLAYER: a game of up to four computers on the same network, one player each. HOST makes a room, JOIN
  finds the rooms and joins one, NAME is the name the room shows. The host's START begins the game on
  every computer, with the host's switches and picture. The first time, your system may ask whether
  TMNT may use the network: allow it on private networks.
- OPTIONS: the cabinet's switches, the region, the controls of each player, the picture (its size, the
  pixels, WIDE: 16:10 as the Steam Deck's screen, 16:9 or 20:9; VSYNC, each frame at the screen's refresh), the
  volume, a sound test; kept in tmnt.ini.
- EXIT.
Choose a turtle, and the game starts with it. In a game, START brings in another player on a turtle
free; a player with no lives left comes back with START too (a continue), choosing his turtle again:
left and right, attack or START (jump: not now). Escape, or START of a player in the game, pauses it (CONTINUE,
RESTART, QUIT GAME). In a MULTIPLAYER game
there is no pause: Escape leaves the game, which goes on for the others.

  Move (player 1)            the arrow keys
  Attack, jump               Z or left Ctrl; X or left Alt
  START                      Enter
  In the menus               the arrows move, attack picks, jump or Escape goes back
  Full screen                F11 or Alt+Enter
  The next kind of pixels    F3

Game controllers: the same, Start is START, Back pauses (Start too, in the game).

The devices
-----------
Each device's games come in this package without assets; a tool beside them makes yours, on this
computer, with your ROM set. What it makes is yours alone: never share it.

Nintendo Switch       TMNT NSP (the folder switch beside it): TMNT.nsp and TMNT Arcade.nsp, with your
                      console's keys (prod.keys). Install them with an installer (DBI, Tinfoil,
                      Goldleaf) on a console with Atmosphere. Each controller is a player, up to four;
                      HOME leaves the game. On TMNT Arcade, minus puts a coin in.
Android               TMNT APK (the folder android beside it): TMNT.apk and TMNT Arcade.apk, signed with
                      a key of yours kept in your user folder (keep it: only an APK with the same key
                      installs over the last one). Copy them to the phone and open them there. The touch
                      screen has a stick where your left thumb lands, ATTACK and JUMP at the right, and
                      START (COIN on TMNT Arcade); a game controller hides them. Back, or START in the
                      game, pauses TMNT; OPTIONS > VIDEO has the pixels and FPS.
Anbernic H700         TMNT Anbernic (the folder anbernic beside it): the folder PORTS, beside the tool or
handhelds             straight into the Roms folder of your handheld's card (choose it as the output
                      folder): the other ports there stay as they are. TMNT and TMNT Arcade show up in
                      the firmware's PORTS. A attacks, B jumps, SELECT puts a coin in on TMNT Arcade;
                      MENU leaves the game.

Start a tool with a double click and drop your files on its window (tmnt.zip, prod.keys, a square
picture of yours for the icon), or run it from a command line: an option it does not know shows the ones
it takes.

Licenses
--------
LICENSE.txt is the project's (BSD-3-Clause). licenses/ holds MAME's and ymfm's (the board's video and
sound follow their behavior and tables), arcade-lan's (BSD-3-Clause: TMNT's MULTIPLAYER, its rooms and
datagrams, github.com/cheviman/arcade-lan), SDL2's (zlib: the window, the sound and the controls, inside
the games), SDL_GameControllerDB's (zlib: the game controllers' mappings, inside the games) and Kenney's (CC0: the letters of the window that asks for the ROM set and of TMNT's menus,
and the pictures of the keys and buttons on its CONTROLS page).

Teenage Mutant Ninja Turtles is a trademark of its owner, and the arcade game is Konami's. This project
is not affiliated with or endorsed by either, nor by Nintendo, Google or Anbernic.
