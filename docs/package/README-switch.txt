TMNT Arcade and TMNT for the Nintendo Switch
============================================

Konami's Teenage Mutant Ninja Turtles (1989), the arcade game for four players, as a native game:
decompiled to C and checked frame by frame against MAME, github.com/cheviman/tmnt-port. TMNT Arcade is
the arcade game as it is; TMNT, the console version (menus of its own, no coins, Splinter Way's modes,
the game over the local network). This is the Switch's package: the games without their assets, ready
for the Homebrew Menu, and the tool that makes your .nsp of them on your computer. Nothing of the game
comes in this package: the games take their graphics and sounds out of your own ROM set.

What you need
-------------
- tmnt.zip, MAME's ROM set of Teenage Mutant Ninja Turtles (World 4 Players, version X), as MAME has it;
- a Switch with Atmosphere.

From the Homebrew Menu (the .nro)
---------------------------------
Copy what is in sd/ to the root of your SD card: switch/tmnt/TMNT.nro and switch/tmnt/TMNT Arcade.nro.
Put tmnt.zip in switch/tmnt/ beside them, and start either game from the Homebrew Menu. The first start
takes the assets out of tmnt.zip into switch/tmnt/assets (some seconds, the screen black); both games use
them from then on. Without tmnt.zip the game says where to put it.

On the HOME menu (the .nsp)
---------------------------
Start the tool for your computer, with a double click:

  Windows   TMNT NSP.exe
  macOS     TMNT NSP (macOS)    (the first time: right click, Open)
  Linux     TMNT NSP (Linux)

Leave it where it is, beside the folder switch (the games' Switch build) and font.bmp. Drop tmnt.zip and
your console's prod.keys on its window (or put them beside the tool), and a 256 x 256 JPEG of yours if you
want your own HOME menu icon. It writes TMNT.nsp and TMNT Arcade.nsp; install them with an installer (DBI,
Tinfoil, Goldleaf) on a console with Atmosphere and signature patches.

What either way makes from your ROM set is yours alone: never share it.

Playing
-------
Each controller is a player, up to four: the stick or the d-pad moves, A attacks, B jumps, + is START (and
pauses TMNT); on TMNT Arcade, minus puts a coin in. HOME leaves the game. Each game keeps its settings and
its log on the SD card: switch/tmnt/ and switch/tmnt-arcade/.

Licenses
--------
LICENSE.txt is the project's (BSD-3-Clause). licenses/ holds MAME's and ymfm's (the board's video and
sound follow their behavior and tables), arcade-lan's (BSD-3-Clause: TMNT's MULTIPLAYER), SDL2's and
SDL_GameControllerDB's (zlib) and Kenney's (CC0: the letters, the controls' pictures).

Teenage Mutant Ninja Turtles is a trademark of its owner, and the arcade game is Konami's; Nintendo Switch
is a trademark of Nintendo. This project is not affiliated with or endorsed by them.
