# The console build: what it will change, and in what order

The port builds two games from the same decompiled program, as segasonic-port and outrunners-port do:

- **TMNT Arcade**: the decompiled game as it is, the same frame by frame as the reference (coins, the test
  mode, the four players), on every platform the port has. It comes first.
- **TMNT**: the console version, the arcade game made for playing at home. The game's functions
  `console/replaced.txt` lists get the console's own versions (`console/*.c`); their definitions in `src/` are
  written as `CONSOLE_ORIGINAL(name)`, so the arcade build runs the same program.

What changes the decompilation (src/, symbols.txt, tools/, runtime/) is done in tmntarcade-decomp and merged
here; this repository adds console/, the ports and their documents.

## The guidelines (Sebas, 04-11-2026)

1. **The order and the names.** The arcade game first, faithful, on every platform ("Arcade"); the console
   version after it, in its own layer: "Splinter Way".
2. **A Konami presentation, as outrunners-port's SEGA one** (its console/presentation.c: the title's SEGA large
   on black, its letters dark until the cars race across and light them up, the engine, the game's voice, START
   skips it). Before the title, a screen of its own with Konami's logo scaled up (Sebas: "escalemos logo y
   konami"): the ribbon and KONAMI only, not "© KONAMI 1989" under them, which is the font's text (Sebas: "el de 1989
   no porque es tipografia"), from the fixed layer's character set (the coin screen's ribbon and KONAMI®, or the
   larger KONAMI 16 pixels high with its larger ribbon that no screen of the scenarios shows),
   drawn with the game's own graphics (the player's assets: nothing of the game's in the repository or the
   executables). What crosses it and what it sounds like (the turtles running across, a voice of the game's: its
   36 speech samples) is to be chosen.
3. **The menu's typeface.** A good-looking one for the menus (to choose: the game's own, from the player's
   assets, or a free one shipped with its license).
4. **The options' background.** The ranking's background, behind the options menu.
5. **The normal mode opens a menu to choose.** The set is the World 4 Players one: each of the four slots is a
   turtle of its own (1 Leonardo, 2 Michelangelo, 3 Donatello, 4 Raphael: `scene_choose_turtles`, "in this set
   each player his own"), so one player always gets Leonardo. With one player, the player chooses the turtle. A
   way that leaves the program as it is: the console puts the player's controls (the coin, the stick, attack and
   jump) on the chosen turtle's slot; with more players each chooses one of the turtles left. To be weighed
   against a turtle select of the program's own, as the 2 players sets have (tmht2p, tmnt2pj: MAME's tmnt.cpp).
6. **A boss rush**, after the normal mode: the bosses one after another. What the decompilation already knows
   for it: an area starts by its number (`area`, 060120, set before the area loads: the scenarios do it with a
   poke), each area's end is its boss (the scenarios' frames per area), the bosses by their kind (Rocksteady 11,
   Bebop 10, Shredder 0F, Krang 65, Baxter Stockman 5C, Roadkill Rodney 53, the rock soldiers 1E and 1D), and the
   enemies come by the scroll (`area_spawn_lists`, `area_scroll_segments`): starting an area at its boss's place
   is what is left to work out.
7. **Ronin**: a mode for one player alone, with the enemies four turtles would have on screen. What the
   decompilation already knows for it: the enemies take the players' slots (`player_slot`, 062FA0:
   `update_player_slots` counts them each frame and finds the least crowded), and the scroll holds while the
   screen has too many enemies (`hold_scroll_for_enemies`); which counts the program scales by the turtles in play
   (how many come, how many at once) is what is left to work out, to make them four's with one turtle.
8. **The Konami code in the menu**: up, up, down, down, left, right, left, right, B, A (B jump and A attack, the
   cabinet's two buttons as a pad has them): it opens a level select, any of the ten areas to start the game in
   (an area starts by its number, `area`, 060120, as the scenarios start them); unlimited lives and continues
   (13); and "everyone the same turtle": every player plays the one chosen (four Leonardos, say). Each slot's
   turtle picks its animations from `turtle_animation_sets` by the turtle object's kind (01B928 "lea $208c0.l,
   A6", 01B92E), so the console gives every turtle object the chosen one's, and swaps the palettes too (Sebas:
   "hacemos un palette swap y listo"): every slot's turtle palette becomes the chosen turtle's, its bandana's
   colour with it.
9. **The picture's shape, from OPTIONS**: 4:3 (the arcade's 320 x 224 as the monitor showed it), wide (16:9) and
   ultra wide (21:9). The video library would draw more of the layers to the sides (the K052109's maps are 512
   pixels wide) and the sprites there; what the game itself keeps up to date beyond its 320 columns (the layers'
   columns drawn as the scroll goes, the objects it leaves off screen) is what is left to work out, as
   outrunners-port did for its WIDE. The arcade build keeps the arcade's picture.
10. **OPTIONS, besides the picture's shape**: a sound test (the songs by name, as the assets name them: area_0 ...
    boss, ending; the effects and the voices, by the sound commands the program sends), the volume (0-10, 5 the
    original's), and the filters the other ports have (sharp, smooth, bilinear, CRT).
11. **The signature**: "@cheviman", the port's author, on the title screen under the game's copyright line, as
    outrunners-port puts it under (C) SEGA 1992 (its console/menu.c, SIGNATURE).
12. **Music on the intro screen** (the console's own screen before the menu): one of the game's songs, which one
    to be chosen (the 24 in assets/sound/music, music.txt).
13. **Lives and continues in Splinter Way**: fixed, 5 lives and 3 continues (the arcade's come from the DIP
    switches, and a coin is a continue as long as there are coins); with the Konami code, unlimited. What the
    decompilation already knows: each player's lives are a BCD byte of the players' records (`PLAYER(...)->lives`,
    060101 + 8 x the player: the scenarios keep them up with a poke), and the continue's countdown and GAME OVER are
    a scene of their own (`scene_continue`).
14. **The DIP switches in OPTIONS**, by the names the test mode's DIP SW SELECT page gives them: the coins (FREE
    PLAY among them), DIFFICULTY (to VERY DIFFICULT), PLAYERS (the lives), DEMO SOUND and SCREEN (flipped), kept
    with the settings and set on the board's switches before the game starts (`tmnt_board.dsw`); Splinter Way's
    fixed lives and continues win over the lives' switch.
15. **Save states**: the game saved and loaded where it is, from the pause menu. The decompilation already keeps
    and compares the whole board's state frame by frame (the work RAM, the video chips' memories, the palette, the
    sound board and its chips), which is what a state has to hold, with the game's own globals and the runtime's.
16. **The region, from OPTIONS.** The program keeps its version in one byte (`version`, 06009F), which the boot
    sets to 0 (0012CE): the title's logo animations are chosen by it (`title_logo_table`, two longs a version,
    004AE0), bit 0 puts TEENAGE MUTANT HERO TURTLES in the copyright and title texts instead of NINJA
    (`hero_title_jobs`, game text 0x13), bit 1 moves the banner (`hero_title_logo_places`). Their sprites and
    texts are in this set's ROMs (the start_title_logo sheet, the fixed layer's font), and so is the warning that
    the game is for use only in Asia (`unused_asia_warning_jobs`, game text 0x0F). The console sets the byte after
    the boot's write: NINJA (World, US), HERO (Europe), and what each of the other values shows is to be seen.
17. **PRESS START instead of INSERT COIN(S).** No coins at home: a player joins with START (the console puts in
    the slot's coin and its attack, as the cabinet's players do), and where the game shows INSERT COIN(S) in a
    player's box (`message_insert_coin`, positioned text 0), the console shows the game's own PRESS START (the
    game's text "PRESS START", which the coin's screen and `message_press_start` already show).
18. **"Original turtles"**: a mode with every turtle in a red bandana, as the original comics drew them: each keeps
    its own moves and weapon, and a palette swap gives the bandana's colours of every turtle's palette Raphael's
    red.
