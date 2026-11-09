TMNT Arcade and TMNT for Anbernic's H700 handhelds
==================================================

Konami's Teenage Mutant Ninja Turtles (1989), the arcade game for four players, as a native game:
decompiled to C and checked frame by frame against MAME, github.com/cheviman/tmnt-port. TMNT Arcade is
the arcade game as it is; TMNT, the console version (menus of its own, no coins, Splinter Way's modes,
the game over the local network). This is the package for the stock firmware of Anbernic's H700
handhelds (RG35XX Plus, RG40XX H and V, RG CubeXX and their kin): the games without their assets, ready
for the card, and the tool that makes your PORTS of them on your computer. Nothing of the game comes in
this package: the games take their graphics and sounds out of your own ROM set.

What you need
-------------
- tmnt.zip, MAME's ROM set of Teenage Mutant Ninja Turtles (World 4 Players, version X), as MAME has it.

Straight to the card
--------------------
The folder anbernic is the card's Roms/PORTS as it is: copy what is in it into Roms/PORTS/ (TMNT.sh, TMNT
Arcade.sh and the folder tmnt; the other ports there stay as they are) and put tmnt.zip in Roms/PORTS/tmnt/.
TMNT and TMNT Arcade are then in the firmware's PORTS. The first start takes the assets out of tmnt.zip into
Roms/PORTS/tmnt/assets (some seconds, the screen black); both games use them from then on. Without tmnt.zip
the game says where to put it (and writes it in tmnt/tmnt.log).

With the tool
-------------
Start the tool for your computer, with a double click:

  Windows   TMNT Anbernic.exe
  macOS     TMNT Anbernic (macOS)    (the first time: right click, Open)
  Linux     TMNT Anbernic (Linux)

Leave it where it is, beside the folder anbernic and font.bmp. Drop tmnt.zip on its window (or put it
beside the tool): it writes the folder PORTS with the assets in it, beside the tool or straight into the
Roms folder of your handheld's card (choose it as the output folder).

What either way makes from your ROM set is yours alone: never share it.

Playing
-------
  d-pad or left stick   move
  A                     attack, pick
  B                     jump, back in the menus
  START                 START (and pauses TMNT)
  SELECT                a coin, on TMNT Arcade
  MENU                  leaves the game
  volume keys           the game's volume
The games keep their settings and their logs (tmnt.log, tmnt-arcade.log) in Roms/PORTS/tmnt/.

Licenses
--------
LICENSE.txt is the project's (BSD-3-Clause). licenses/ holds MAME's and ymfm's (the board's video and
sound follow their behavior and tables), arcade-lan's (BSD-3-Clause: TMNT's MULTIPLAYER), SDL2's and
SDL_GameControllerDB's (zlib; the handheld's games take the firmware's SDL2) and Kenney's (CC0: the
letters, the controls' pictures).

Teenage Mutant Ninja Turtles is a trademark of its owner, and the arcade game is Konami's. This project is
not affiliated with or endorsed by them, nor by Anbernic.
