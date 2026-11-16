TMNT testing: the next release's changes, for the players to try before it comes out. It is not a release: it is
made again at each change, and something in it may not work yet. Tell what you find in the issues
(github.com/cheviman/tmnt-port/issues), with the version TMNT's OPTIONS shows at the bottom right (V...-TEST...).
As in a release, nothing of the game comes in these packages: keep your own `tmnt.zip` (MAME's ROM set, World 4
Players) beside the game, and it takes its assets out of it the first time it starts.

**To try**

- **BOSS RUSH without continues** (issue 5): its 5 lives and no more. When a turtle has none left, START no longer
  brings it back: the countdown runs out, and with no one playing, GAME OVER. NORMAL keeps its 3 continues.
- **WIDE, the highway's cars** (issue 14): once a car has driven past, nothing of it stays at the picture's sides.
- **WIDE, what comes in at the right** (a video review): the traffic cones, the signs, the oil drums, the hydrants,
  the pizza boxes, the highway's tires and barriers, the doors, manholes, turrets and spiked poles no longer show up
  from nothing in the picture's right side: they are already there when the side comes to them. The motorbikes, the
  helicopters, the rocket soldiers and the skateboards' soldiers come in from past the picture's edge.
- **WIDE, the bosses** (a video review): Rocksteady, Bebop, Shredder and Baxter Stockman fight in the picture's
  sides too, where the turtles go, no longer only in the arcade's 4:3 part of it.
- **The Switch's picture** (issue 19): it fits the screen, docked and handheld, with the TV at 1080p or 720p. TMNT
  no longer goes full screen there: the Switch's SDL made that a 1920 x 1080 picture, of which the 720p screen
  showed a part.
- **SPECIAL, attack and jump at once** (issue 13): OPTIONS > CONTROLS has it, for the turtles' special attack on
  one key or button: a controller's X, C on the keyboard (Q for player 2), or whatever each player binds to it.
  TMNT Arcade takes a controller's X and C too. In the menus it picks, as attack does.
- **The mouse pointer** (issue 8): it hides over the game after two seconds without moving, and comes back when it
  moves.
- **The Technodrome's lift** (issue 21): the game no longer closes when the lift lands with a turtle pushing the
  scroll. The shaft was drawn from past its map's end there, which the board reads and draws out of sight; the game
  took those bytes for missing, and stopped. Both games, TMNT and TMNT Arcade. The first run takes the assets out of
  your `tmnt.zip` once more (they now hold the rest of the program's bytes the game may read so).
- **A log on the computers** (issue 21): started from Explorer, the Finder or a launcher, TMNT writes `tmnt.log`
  (TMNT Arcade `tmnt-arcade.log`) where it keeps its files: beside the game on Windows, in
  `~/Library/Application Support/TMNT` on macOS, `~/.local/share/TMNT` on Linux; the last two runs' too
  (`tmnt.1.log`, `tmnt.2.log`). If the game closes by itself, that file says why: attach it to the issue.
- **The attract mode** (issue 20): after the Konami presentation, TMNT plays the arcade's own attract mode (the
  story, the title, the demos, the turtles' profiles, the best players), and after 30 seconds on the menu with
  nothing pressed it goes back to it. Any key or button opens the menu; one during the presentation skips both.
- **COINS** (issue 17): OPTIONS > COINS ON plays ARCADE with coins, as the cabinet: the boxes say INSERT COIN, the
  keys 5, 6, 7 and 8 (or a controller's Back) put a coin in for players 1 to 4, and a coin is that player's START
  with a credit: it picks his turtle, brings him in or continues, and adds lives to a turtle playing. START alone
  needs credits. Off (the default), free play as before. Not in SPLINTER WAY nor MULTIPLAYER.

The game is free, and stays free. To support the decompilations and the ports, and have a say in what comes
next: [patreon.com/ChevimanPorts](https://www.patreon.com/ChevimanPorts). Entirely optional.

Teenage Mutant Ninja Turtles is a trademark of its owner, and the arcade game is Konami's; this project is not
affiliated with or endorsed by either, nor by Nintendo, Google, Apple or Anbernic.
