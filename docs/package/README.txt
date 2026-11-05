TMNT Arcade and TMNT
====================

Konami's Teenage Mutant Ninja Turtles (1989), the arcade game for four players, as a native game:
decompiled to C and checked frame by frame against MAME, github.com/cheviman/tmnt-port. Nothing of the
game comes in this package: it takes the game's graphics and sounds out of your own ROM set.

Two games come in it, the same program:
- TMNT Arcade.exe, the arcade game as it is: coins, the test mode, the four players.
- TMNT.exe, the console version: the game made for playing at home, with menus of its own, no coins
  (START brings a player in) and Splinter Way, the modes of the port's own.

What you need
-------------
tmnt.zip, MAME's ROM set of Teenage Mutant Ninja Turtles (World 4 Players, version X), as MAME has it.
Keep it beside the games, or drop it on the game's window when it asks for it; a folder rom beside the
games with the set's files (or tmnt.zip) in it works too. Keep font.bmp beside the games: the window that
asks for the set and TMNT's menus write with it.

The first time a game starts it takes its assets out of the ROM set (a few seconds) and keeps them in the
folder assets beside it; from then on both start from them and never open the ROM set again. The assets
come from your ROM set, so they are yours alone: never share them.

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

From a command line, "TMNT Arcade.exe" --service starts the game in its test mode, as the cabinet's
switch does.

Playing TMNT
------------
The menu under the title's logo: ARCADE (the arcade game, its lives the PLAYERS switch's), SPLINTER WAY
(NORMAL: 5 lives and 3 continues; more modes to come), OPTIONS (the cabinet's switches, the region, the
volume, the pixels; kept in tmnt.ini beside the game) and EXIT. Choose a turtle, and the game starts with
it. In a game, START brings in another player (or the same one again, a continue) on a turtle free.

  Move (player 1)            the arrow keys
  Attack, jump               Z or left Ctrl; X or left Alt
  START                      Enter
  In the menus               the arrows move, attack picks, jump or Escape goes back
  Full screen                F11 or Alt+Enter
  The next kind of pixels    F3

Game controllers: the same, Start is START.

Licenses
--------
LICENSE.txt is the project's (BSD-3-Clause). licenses/ holds MAME's and ymfm's (the board's video and
sound follow their behavior and tables), SDL2's (zlib: the window, the sound and the controls, inside
the games) and Kenney's (CC0: the letters of the window that asks for the ROM set and of TMNT's menus).

Teenage Mutant Ninja Turtles is a trademark of its owner, and the arcade game is Konami's. This project
is not affiliated with or endorsed by either.
