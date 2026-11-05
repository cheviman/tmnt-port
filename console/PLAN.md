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

1. **The order and the names.** The arcade game first, faithful, on every platform ("TMNT Arcade"); the console
   version after it, in its own layer, whose executable is just "TMNT" (Sebas: "tmnt solo"), as outrunners-port's
   console one is just "OutRunners". Its menu: ARCADE (the arcade game at home), SPLINTER WAY (the modes of our own,
   as outrunners-port's & KNUCKLES: Sebas, "el splinter way es como el & knuckles del outrunners"), OPTIONS, EXIT.
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
12. **Music on the menu's screen**: the staff roll's song, staff_roll (Sebas: "staff_roll y pone esta para musica
    del menu").
13. **Lives and continues in Splinter Way**: fixed, 5 lives and 3 continues (the arcade's come from the DIP
    switches, and a coin is a continue as long as there are coins); with the Konami code, unlimited. What the
    decompilation already knows: each player's lives are a BCD byte of the players' records (`PLAYER(...)->lives`,
    060101 + 8 x the player: the scenarios keep them up with a poke), and the continue's countdown and GAME OVER are
    a scene of their own (`scene_continue`).
14. **The DIP switches in OPTIONS**, by the names the test mode's DIP SW SELECT page gives them: DIFFICULTY (EASY,
    NORMAL, HARD, VERY HARD: the test mode's DIFFICULT and VERY DIFFICULT, in Sebas's words), the lives (its PLAYERS, shown as LIVES: Sebas found PLAYERS 1, 2, 3, 5 confusing), DEMO SOUND
    and SCREEN (flipped), kept with the settings and set on the board's switches before the game starts
    (`tmnt_board.dsw`); not the coins (Sebas: "si estamos sacando las monedas"); Splinter Way's fixed lives and
    continues win over the lives' switch.
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
19. **The menu's screen**: the title's TMNT logo (the TEENAGE MUTANT NINJA banner and TURTLES, the title's own
    sprites from the player's assets) on its own, without the title's texts (the copyright lines), on its light
    blue background (Sebas, after seeing it on black: "ponelo en el celeste"); the menu under it, and under the logo
    "© KONAMI 1989" (the title's own text) with "@cheviman" under it (Sebas).

## What was done (04-11-2026)

The names (Sebas): the console version is **TMNT** (`TMNT.exe`, its window "TMNT"), as outrunners-port's is
OutRunners; **Splinter Way** is its modes of the port's own, a menu of its own, as & KNUCKLES is OutRunners'.

- **The build.** `console/replaced.txt` lists four of the game's functions; `py tools/decompile.py` writes their
  definitions as `CONSOLE_ORIGINAL(name)` (src/ differs only there, and in their declarations in game.h):
  `run_attract`, `title_turtles_slide_in`, `draw_best_players`, `message_insert_coin`.
  CMake builds `tmnt_console_game` (the decompiled C, the runtime's game files and console/*.c, `TMNT_CONSOLE`),
  `TMNT` (ports/sdl2 with the console) and `tmnt_console_headless` (`--actions FILE`: the menus' actions and the
  players' START by frame; `--video-dump FRAME ... --video-dir DIR`: the frames with the console's picture
  over them, as BMP, and where the console stands on the standard output; `tests/console/` holds the actions of a
  few runs). The Windows package holds `TMNT.exe` beside `TMNT Arcade.exe`. TMNT Arcade is the same program: the
  17 scenarios give the same frames.
- **How the console draws.** Its own picture goes over each frame the video drew (`console_overlay`, which both
  ports call): the menus in the game's own letters (the fixed layer's 8 x 8 font, in its texts' white and light
  blue: Sebas, "las celeste y blancas van", after Kenney Mini's looked too large), with an '@' of the console's
  own (the font's '@' is its space) and "@cheviman" in small letters of its own; the game's pictures from the
  player's assets as the board has them (the tile and sprite memories, the palette RAM), drawn the way
  runtime/src/video.c draws them (console/draw.c): nothing of the game's in the repository or the executables.
  Kenney Mini (CC0, `font.bmp`) is still loaded and nothing draws with it: to take out.
- **The front end** runs in the attract mode (`run_attract`), on the game's own screens, each held while its page
  is open: the title's logo for the menu (attract step 3), BEST 10 PLAYERS' picture for OPTIONS (step 6), the
  coin's screen for the turtle choice (step 1); the game starts from there as the cabinet's does.
- **2: the Konami presentation** at power-on (console/presentation.c): the fixed layer's larger Konami logo (game
  text 16, the ribbon and KONAMI 16 pixels high, which no screen of the game shows; no "© KONAMI 1989") twice its
  size, in the palette's first bank (the coin's screen's logo colours), black until the four turtles run across
  it and light it (Sebas: not grey before them): the game's own objects (the turtles' slots) with their own walk
  (`turtle_animation_sets`, routine 1, state 0), stepped on and drawn by the game's sprite engine with area 0's
  palettes, drawn again by the console over the logo. Then the coin's sound (PCM effect 86; Sebas, after hearing
  it and speech A2 in each other's place); START or attack skips it.
- **19, 11: the menu's screen.** The title's logo on its light blue, `title_turtles_slide_in` printing of the
  title's copyright lines only "© KONAMI 1989" (the game's text, its colour, a row higher than the title's: room
  under it), and "@cheviman" under it in small letters of the console's own (5 pixels high). The menu under the
  logo, in the game's own letters (the white and light blue ones; the line chosen yellow): ARCADE, SPLINTER WAY
  (NORMAL, BOSS RUSH, RONIN: the last two do nothing yet; Sebas, no "SOON" on them), OPTIONS, EXIT; speech A2 (the uPD7759's sample 2, speech_02: Sebas)
  for a line moved to, a setting or the turtle changed (he found the name entry's letter, 1D, horrible), none for a
  line picked or a page left.
- **12: the menus' music** is the staff roll's song (70), which the game plays once: the menus ask for it again
  when it ends (the music's channel records in the sound board's RAM all free; Sebas: "no loopea"); the turtle
  choice has the coin's screen's (6F), which that screen starts itself, and the menu's starts again back from it.
- **17: no coins.** A player's START puts the coin and then the attack in on his slot (console_controls: the
  board's inputs, as the cabinet's players do), and every box that would say INSERT COIN(S) shows the game's own
  PRESS START, blinking (`message_insert_coin` calls `message_press_start`); a box whose START would bring no one
  in (Splinter Way's continues used) has its line cleared.
- **5's way, for one player: the turtle choice.** ARCADE and NORMAL open the coin's screen with its four boxes
  and the four turtles' pictures (the switch's free play set for it: `free_play`), the ones not chosen darkened;
  left and right choose, attack or START picks: the player's controls go on that turtle's slot (Raphael's is
  4UP), and its coin and attack start the game. In a game, another player's START takes the first slot free.
- **13: Splinter Way's NORMAL**: 5 lives a coin (`lives_per_credit` 5 while it plays: the arcade's 5 lives), and 3
  continues (a slot's coins after its first); ARCADE gives the PLAYERS switch's lives and continues while START
  is pressed. Checked in a run of 32000 frames: a coin and three continues, then the countdown runs out, GAME
  OVER, the ranking, the menu.
- **4, 14, 16, 10: OPTIONS** over BEST 10 PLAYERS' picture without its table (`draw_best_players`), darkened
  behind the lines: DIFFICULTY, LIVES (the test mode's PLAYERS: the lives a credit gives, 1, 2, 3 or 5), DEMO
  SOUND, SCREEN (the test mode's DIP SW SELECT names and values), REGION, VOLUME (0-10, 5 the game's), FILTER
  (SHARP, SMOOTH, BILINEAR, CRT); the port's release at the bottom right ("V0.1.0", CMake's project version, as
  outrunners-port's). No COIN1 (Sebas: no coins at home): the coins' switches stay at 1 coin 1 credit,
  so that START's coin is a credit. The switches go on the board before the boot reads
  them (`tmnt_board.dsw`) and, changed, into the copies the boot made when OPTIONS closes; all of it kept in
  `tmnt.ini` beside the executable.
- **16: the region**, the `version` byte (06009F), set when the menu's title starts: 0 NINJA (TEENAGE MUTANT
  NINJA TURTLES: the World and US sets'), 1 HERO (TEENAGE MUTANT HERO TURTLES: the UK set's), 2 JAPAN (NINJA with
  the Japanese subtitle スーパー亀忍者 under TURTLES, the banner a little higher: the Japanese set's), 3 HERO JAPAN
  (HERO with the subtitle: no set's). OPTIONS offers these four: a value past 3 would take its logo's
  animations past `title_logo_animations`' four pairs.
- **TMNT's window**: Enter or a controller's Start is START; the arrows, attack (Z, A) and jump or Escape (X, B)
  move about the menus, each player's stick and buttons too (console_controls takes a player's newly pressed
  controls as the menus' actions, besides the keys' events); F3 the next kind of pixels, F11 full screen; EXIT
  closes it. The window without the picture's first 6 columns (the K052109's layers A and B show the end of their
  maps there, which the cabinet's bezel hid; Sebas: "no la quiero").

- **10, OPTIONS as outrunners-port's** (Sebas: "lo mismo que tiene outrunners"): CONTROLS, DIFFICULTY, LIVES, DEMO
  SOUND, SCREEN, REGION, VIDEO, VOLUME, SOUND TEST, FPS, BACK. VIDEO: WINDOW (1X-6X), FULL SCREEN, PIXELS, WIDE. SOUND
  TEST: MUSIC by the names music.txt gives the songs (the title music too), EFFECT (FM 01-41, PCM 80-93), VOICE (the
  uPD7759's samples 00-35, speech_NN), STOP; the menus' song again after it. FPS: the frames a second at the top right
  and in the window's title. Settings in tmnt.ini (wide=, fps= besides).
- **CONTROLS**: each of the four players' keys and game controller buttons (ports/sdl2/controls.c, controls.ini beside
  the executable, as outrunners-port's): < PLAYER N > on top, UP DOWN LEFT RIGHT ATTACK JUMP START with the pictures of
  their keys and first button (Kenney's Input Prompts Pixel, CC0, input_prompts.bmp), DEFAULTS, BACK; a control picked
  takes the next key or button (Escape leaves it). Defaults: MAME's keys for the four players, every pad's A B and Start.
  The menus take the arrows, Enter and Escape besides whatever the players' are.
- **The pause** (Escape or a pad's Back in a game; START brings players in): the frame in grey under PAUSE, CONTINUE,
  RESTART (the same game, the same turtle for the same player), QUIT GAME (the game's own back_to_attract after the
  play, the layers and the players cleared: the menu).
- **9, WIDE** (console/wide.c): 16:9 (48 columns more at each side, the window 398) and 20:9 (88, 496), in a game's
  play only (scenes 4 and 5; the story screens, the continue, the ranking and the menus keep the board's 320 between
  black bars). The video draws the layers and the sprites past the board's 320 (tmnt_video_set_wide; with it the
  sprites' x takes a tenth bit, byte 6's bit 2, a zoom bit tmnt never sets, so none shows at the other side); the
  console writes the pieces of the objects that show there (draw_sprites, at most 128), the columns and the row's
  blocks the game does not draw that far (draw_new_row_and_column and its floor variant: their tiles only, from the
  same map, never past the map's ends nor over what the ring shows), and has the video leave black what lies past a
  map's ends. Where a side has no map past the board's screen (an area's start at its left, its end at its right) the
  picture shows what there is of it and the rest at the other side, back to the middle as the scroll brings the map
  (Sebas: "recorta de la izquierda y corre"; the game is the same). To come: the menus WIDE too (Sebas: "el menu tmb
  tiene que tener wide"); the enemies come in some 16 pixels past the board's edge and go out 32 past it, so in the
  bands they show up and vanish there (Sebas takes changes to the play where WIDE needs them).
- **tmnt_console_headless --arcade-start** runs the arcade's scenarios in the console's build (the arcade's attract
  and controls; `--wide 1|2` for their games), and `pause`, `quit`, `restart` in --actions.

Left for later: BOSS RUSH and RONIN (6, 7), the Konami code (8), save states (15), "Original turtles" (18); with more
players, each choosing a turtle of those left (5: today each START after the first takes the first slot free); Kenney
Mini still loaded and nothing drawing with it. Found on the way, in the decompilation (TMNT Arcade too): with no button
pressed through area 0's intro, at its end object 45's animation (02DF46) runs on into 02DF50, whose "00 FF" jumps to
001800, a frame in the boot's code; the reference reads it, the game stops there (the assets have no piece at 001800):
fixed in tmntarcade-decomp (not committed yet), to be merged here.
