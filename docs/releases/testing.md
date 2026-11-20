TMNT testing: the next release's changes, for the players to try before it comes out. It is not a release: it is
made again at each change, and something in it may not work yet. Tell what you find in the issues
(github.com/cheviman/tmnt-port/issues), with the version TMNT's OPTIONS shows at the bottom right (V...-TEST...).
As in a release, nothing of the game comes in these packages: keep your own `tmnt.zip` (MAME's ROM set, World 4
Players) beside the game, and it takes its assets out of it the first time it starts.

**To try**

- **Slow devices** (the Switch first; the Anbernic and a Raspberry Pi 3 too): the game draws its picture some 13
  times faster, the same picture pixel for pixel. Where drawing a frame took longer than its time, the game and its
  sound slowed down together; a frame now leaves most of its time free. No frame is left out. Measured on a
  computer only (the drawing from 2.4 ms a frame to 0.2 in WIDE 16:9). Not tried on a Switch nor on any other
  device: tell how it goes there, docked and handheld, and whether the sound still breaks up anywhere.

The game is free, and stays free. To support the decompilations and the ports, and have a say in what comes
next: [patreon.com/ChevimanPorts](https://www.patreon.com/ChevimanPorts). Entirely optional.

Teenage Mutant Ninja Turtles is a trademark of its owner, and the arcade game is Konami's; this project is not
affiliated with or endorsed by either, nor by Nintendo, Google, Apple, Anbernic or Raspberry Pi.
