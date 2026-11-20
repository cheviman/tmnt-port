TMNT testing: the next release's changes, for the players to try before it comes out. It is not a release: it is
made again at each change, and something in it may not work yet. Tell what you find in the issues
(github.com/cheviman/tmnt-port/issues), with the version TMNT's OPTIONS shows at the bottom right (V...-TEST...).
As in a release, nothing of the game comes in these packages: keep your own `tmnt.zip` (MAME's ROM set, World 4
Players) beside the game, and it takes its assets out of it the first time it starts.

**To try**

- **SDL 3** (the window, the sound and the controls: SDL 3.4.20 where SDL 2 was): Windows, Linux, macOS and
  Android. Everything should be as it was, so tell what is not: the sound, the full screen (F11, Alt+Enter, macOS's
  green button), VSYNC, the pause, your game controllers (the log says what SDL finds of each), the touch controls'
  size. Two things do change. On Windows the window is at the screen's own pixels: on a desktop scaled to 125% or
  150% it is smaller and sharp where it was stretched and soft; OPTIONS > VIDEO > WINDOW makes it larger. And on a
  computer a Nintendo controller's buttons go by their place, as an Xbox controller's: the button at the bottom
  attacks.
- **The Nintendo Switch on SDL 3**: devkitPro has no SDL 3 of its own yet, so the games carry its branch of it (SDL
  3.4.0, not released). Not yet tried on a Switch: the picture docked and handheld, the sound and the controllers are
  the first things to tell. If it does not start, 1.0.7 is the one that does.
- **Anbernic**: its games keep the firmware's SDL 2, the same source underneath. Not yet tried on a handheld.
- **Raspberry Pi** (new): `tmnt-raspberrypi-arm64.tar.gz`, for a Raspberry Pi 3 or later (3, Zero 2 W, 4, 400, 5,
  500) with the 64-bit Raspberry Pi OS, Debian 12 "Bookworm" or later. From the desktop or with none (Raspberry Pi
  OS Lite). The game plays the same frame by frame under emulation; not yet tried on a Raspberry Pi: the picture,
  the sound, the controllers and the speed on a Pi 3 are to be seen there.
- **iPhone and iPad** (new): `tmnt-ios.zip`, the two games as .ipa files to install yourself (AltStore, SideStore,
  Sideloadly: its README says how), iOS 13 or later. Never on the App Store. Built for the first time by this
  build, and not yet tried on an iPhone or iPad: tell whether it starts, and send `tmnt.log` from the game's folder
  in the Files app if it does not. MULTIPLAYER there is not expected to find the rooms yet.
- **Donatello lands with his bo** (issue 23): falling into areas 2, 3 and 7, his landing pose had a piece of
  Raphael's sai where the bo's lower end goes (one bit of the arcade board's own data; MAME shows it too). TMNT puts
  it right; TMNT Arcade keeps the board's data.

The game is free, and stays free. To support the decompilations and the ports, and have a say in what comes
next: [patreon.com/ChevimanPorts](https://www.patreon.com/ChevimanPorts). Entirely optional.

Teenage Mutant Ninja Turtles is a trademark of its owner, and the arcade game is Konami's; this project is not
affiliated with or endorsed by either, nor by Nintendo, Google, Apple, Anbernic or Raspberry Pi.
