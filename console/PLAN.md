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
20. **Multiplayer over the local network, for the four** (Sebas: "un modo multi por lan local, para los 4"): up to
    four devices on a LAN playing one game, each player his turtle. A client / server that carries the inputs (Sebas:
    "un codigo para cliente / servidor que lea los input"): the server (the device that hosts) takes each client's
    controls every frame and sends every device the frame's inputs of the four players; each runs the same game with
    them, which the decompilation keeps deterministic (the same inputs give the same frames: the state's hash per
    frame, already in the ports, tells a device that has gone apart). The game held while a device is not heard.
    Generic, for the other projects too (Sebas: "para hacerlo generico para poder usarlo en otros proyectos"): a C
    library in a project of its own (Sebas: "yo diria en otro proyecto"; its own repository, which each port takes as
    a submodule, as the decompilations take arcade-decomp), with no game in it: it carries each player's controls as
    a few bytes a frame, finds the rooms on the LAN, joins them, holds the frames while a device is not heard, and
    compares the state's hash each game gives it; a port gives it its controls, runs its frames with the inputs it
    hands back, and draws its own menus (HOST, JOIN, a player's NAME). outrunners-port already has a LAN game (Sebas:
    "el outrunners ya tenia lan"; console/network.c, net_udp.c, net.h): the arcade's link of cabinets over UDP, each
    device a cabinet running its own cars and sending its race records to the others, nothing waiting frame by
    frame. Its rooms (a host's beacon every half second, JOIN, the list of devices, a device unheard for a third of a
    second or gone after 3), its UDP and its datagrams written byte by byte (devices need not be alike) are what the
    library takes; what TMNT adds is the four players' inputs a frame, every device waiting for them (one cabinet, as
    the arcade's TMNT is). OutRunners could carry its records through the same rooms and UDP later.

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
- **The pause** (Escape or a pad's Back in a game, or START of a player whose turtle plays; another's START brings him
  in): the frame in grey under PAUSE, CONTINUE,
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
  (Sebas: "recorta de la izquierda y corre"; the game is the same).
- **tmnt_console_headless --arcade-start** runs the arcade's scenarios in the console's build (the arcade's attract
  and controls; `--wide 1|2` for their games), and `pause`, `quit`, `restart` in --actions.

## What was done (05-11-2026)

- **tmntarcade-decomp merged** (area 0's story untouched: 001800 a piece of its own, in the assets' `code_read`
  group; the README's screenshots): the first run's layout written again (`tools/extract_layout.py`: the code read
  as data in `code_read` too), 420 of 420 files the same as extract_assets.py's; 18 scenarios identical, picture and
  sound.
- **9, WIDE in the pause** (Sebas: "en la pausa se rompe el wide tmb"): the console draws over the whole frame the
  video drew (`console_picture`: the board's 320 or WIDE's wider frame, its sides; x from -left to 319 + right), so
  the pause stops on the wider frame, all of it in grey, PAUSE and its lines in the window's middle.
- **9, WIDE in the menus** (Sebas: "el menu tmb tiene que tener wide"): the title's light blue, Splinter Way's page,
  the turtle choice and OPTIONS as wide as the play; OPTIONS' background (BEST 10 PLAYERS': a row of one tile each
  on layer B) put in the ring's other columns, which the sides show; the line chosen centred in the window's middle,
  the release at the wider picture's right. The presentation (on black) keeps the board's 320.
- **9, the enemies past WIDE's sides** (Sebas takes changes to the play where WIDE needs them): an enemy of the spawn
  lists comes in its kind's margin past the *wider* screen's side (the game's 16 pixels past the board's), and an
  object is off the screen past the wider screen's sides (`spawn_enemies`, `update_object_screen_place` in the
  console's versions), so they walk the sides' columns more to come in and no longer show up or vanish in them.
- **9, the layers rebuilt past the board** (Sebas: "en la parte izquierda, hay que reconstruir en algunas
  secciones"): when the game draws a whole layer (an area's start, an exit's picture) it draws the ring from a page
  of 256 pixels, and the wider screen's left side may lie before it, showing what the ring holds of the map 512
  pixels on; the console draws both sides' columns again from the layer's map at that frame's end, and when WIDE's
  sides come (the play after the story screens). And only the layers the area draws as maps count (those
  `start_area_layers` draws whole, or the scroll draws column by column): the other one's record keeps what an earlier
  area left in it, and in the areas of one map (the street, the car street, the highway, the Technodrome...) it held
  the picture's left side to some 26 pixels and cut it black there. `tmnt_console_headless --wide-check` says, each
  frame, which of the sides' columns are not their map's (with every area's scenario at 20:9 only some the game
  draws itself remain: area 1's exit, its kidnapping room; area 8's lift, which do not show).
- **5, a turtle for each player**: on the turtle choice the player who opened it is in, and any other comes in with
  START or attack (PRESS START TO JOIN under the pictures), each on a turtle none has picked: LEFT and RIGHT move
  among those, attack or START picks, jump takes the pick back (or leaves: the one who opened it, back to the menu).
  Each player in a frame of his box's colour with his 1P to 4P under it (one player alone: yellow and the turtle's
  name, as before). The game starts when every player in has picked, each on his turtle's slot; RESTART picks again
  what each played (those who came in during the game too). In a game, a START still takes the first slot free.
  `tmnt_console_headless --actions`: an action with a PLAYER (`720 right 2`) is that player's own stick or button;
  `tests/console/two_players.txt`.

- **Sebas's tries of the batch** (05-11, the same day): the area's start with WIDE's columns all at the right (Sebas:
  "correr todo para la derecha para que no aparezca"; the left side shows nothing before the board's column 0 where
  the area's scroll starts, area 0's 32 blank columns before the building); the title's TURTLES waiting past the
  wider screen's edge while the banner comes down (its T showed in 16:9's side); only the Foot soldiers that walk in
  (kinds 00-0D with no way in) come in past the wider screen's sides, the doors, the scenery and the soldiers coming
  out of a door or a window where the game puts them (Sebas: "se fue de offset los ninjas que salen de las
  puertas"); the menu takes no action while the title's logo comes in, and attack, START or jump puts it in place
  at once (Sebas: Enter picked ARCADE; "no tendria que moverse nada mientras se esta animando"); OPTIONS without
  SCREEN (the screen always the right way up) and FPS in VIDEO (Sebas).

- **9, the turtles into WIDE's sides** (Sebas: "no puedo ir mas de esta parte para la izquierda"): a turtle's x is the
  screen's (the K051960's: the board 0x60 to 0x1A0) and the game keeps it between 0x90 and 0x160 (turtle_run_all),
  scrolls when one passes 0x160 or, where the area lets it, 0x91 (turtle_move), holds the scroll while another is at
  the other edge (turtle_other_at_edge) and keeps a knocked one in (the four slides, turtle_knockback_arc,
  turtle_walk_off_screen). The console's versions (console/wide_turtles.c: the same C with those limits moved) add
  the left side's columns and the right side's, at most 48 (the game draws the floor map only 8 pixels past the
  board's right edge, while at the left its ring keeps the columns the scroll passed: a turtle there reads its walls
  and holes as on the board). Measured walking left in area 0: 0x90 in 4:3, 0x60 in 16:9, 0x38 in 20:9, the same
  distance from the picture's edge; at the right 0x190 (then: the whole side, see below). Without WIDE the game's
  own run.

- **8, the Konami code's level select, in the open** (Sebas: "habilita el menu sin poner el codigo, y despues lo
  escondemos", "pone konami code en el raiz"): the menu's KONAMI CODE, after OPTIONS: < AREA 1-10 > by the areas'
  names (BUILDING ... KRANG), < LIVES INFINITE | NORMAL > (infinite at first), START, BACK; Splinter Way's game on the
  turtle choice (AREA N - CHOOSE YOUR TURTLE), its area set when the game has put its own 0 and not yet read it (the
  game's turtle choice, scene 1, or the area's set-up before its first step: area_setup_variables takes the stage
  by it), as the scenarios poke it; infinite lives kept at 5, the box drawn again; RESTART starts the same area.
  `tests/console/konami_code.txt`: area 5. The code itself (06-11, Sebas: "agreguemos el konami code"): on the menu
  up, up, down, down, left, right, left, right, B (the jump, BACK), A (the attack, ACCEPT), from any player's keys or
  game controller, opens the level select with the coin's sound (PCM 86), and the menu's KONAMI CODE line is gone;
  the code's ups and downs take the menu's line there and back, its A picks nothing, and BACK from the level select
  goes back to that line. The last ten presses kept, one a frame (several at once, none of the code's).
- **18, "Original turtles", tried in KONAMI CODE** (< ORIGINAL TURTLES ON | OFF >): the turtles' sprites show with
  the palette RAM's sprite banks 0-3 (colours 16-19 x 16, by their animations' pieces), whose pens 10 and 11 are
  the bandana's; each frame Raphael's two go into the other three as the palette RAM has them, faded with them:
  every turtle keeps its moves and weapon, in Raphael's red.
- **8, "everyone the same turtle", in KONAMI CODE** (< SAME TURTLE ON | OFF >; the turtle choice's heading AREA N -
  EVERYONE THE SAME TURTLE): every player plays the turtle the first one picks, its moves, weapon and colours, each
  on his own slot (the others pick their slots as before). A turtle object's kind is both the turtle
  (turtle_animation_sets, whose pieces carry its palette; turtle_combos, turtle_attack_firsts,
  turtle_hit_animation_tables, turtle_hit_waits, the walk speeds, Raphael's own states, Michelangelo's and
  Donatello's turn in turtle_attack_over) and its player (his lives and health, his box, his prompts, the bits of
  his score and knock-out, where he starts and comes in): the kind stays the player's, and the console's versions
  of the ten functions that take the turtle's tables by it (console/same_turtle.c, with turtle_run_all in
  wide_turtles.c: TURTLE_KIND) take them by the chosen one. The boxes keep each slot's own turtle's face.

- **Sebas's second tries** (05-11, night): the play's start keeps the board's 320 until the area has drawn its layers
  (start_area_layers: the first wide frame was black, its boxes moved), and the arcade's first column shown at an
  area's start is the board's 7th (the first 6 the K052109's, which the bezel hid): with WIDE all at the right they
  are the picture's left edge, black (16:9's window leaves them out, 20:9's shows them); the HURRY arrow at the
  wider screen's right as at the board's (its x 0x158, object 49); a map the game loops (the skateboards' road: its x
  back by 90 pixels every 85 frames) takes no room, so the picture no longer moves at each loop (Sebas: "se mueve
  raro, como que pierde foco"); the turtles walk the right side too ("le falta un poco para la derecha"): the console
  writes the floor map's cells of the columns it draws past the game's (its floor variant's, the same cells), and
  the sides' room for them stops where the scroll's segment does (the layers' x_min and x_max: in Krang's room,
  which the scroll holds, the arcade's limit at the right; Sebas: "necesita un tope, porque se pasa"); "Original
  turtles" on the turtle choice too (the four pictures' bandana pens 1-3) and in the boxes' faces (their cells in
  Raphael's box's bank, the labels in their own colours).

- **7, RONIN** (console/modes.c): one player alone against what four turtles would have. The game's enemies come
  by its difficulty (update_difficulty: the DIP switch's, one more for each player in play, the play's time without
  a knock-out, 7 at most), which picks the spawn lists' records that come (a record's flags' low nibble) and how many
  enemies hold the scroll (enemy_limits by stage and difficulty): RONIN's counts the four players (NORMAL's 2, RONIN's
  5 in area 0, measured); how the enemies share the turtles stays the one turtle's. No other player comes in (the
  choice, a START in the game), and only his box is drawn (draw_player_boxes with his bit; the other boxes' cells
  kept clear: Sebas, "en ronin, saquemos los cuadros"). `tests/console/ronin.txt`.
- **6, BOSS RUSH, a first version** (console/modes.c): from area 1, each area crossed to its boss by the game's own
  play, out of sight and as fast as the window goes (console_fast_forward: no pacing, a frame in eight shown, no
  sound; BOSS RUSH and the area on black): the spawn lists' Foot soldiers passed, those the doors and the rest let
  out off the screen for the game (which takes them off its own way; one riding stays), the turtles walking right
  and attacking now and then, their health kept full. A boss's coming (the music changing to a boss's: 0xFE, 0x64,
  0x6A) gives the players the play; the area over, the next is crossed. Measured from each area's start: Rocksteady
  (area 1) in ~1000 frames, Baxter (3) ~5700, Rocksteady and Bebop (4) at once, Shredder (9) ~7000 (Krang fought on
  the way); the street (2), the highway (5) and the building site (7), the Technodrome (8) stop in a section the
  crossing does not get through (the scroll unmoved for 15 seconds of the play: the players play from there), the
  skateboards (6, no boss) after 10000 frames (then: to their end, see below). Learned on the way: taking the
  soldiers off outright left a skateboard on nothing (the game stopped reading its code as data); invulnerable turtles with the soldiers off the screen
  stopped it in area 7; area 6 set up as 7 left the skateboards' state to area 7's Mousers. Still to see, playing it:
  a stop in enemy_mouser_think (0510F8) in area 7 after the tests' area 6 ended by a poke (0600BD) mid-crossing;
  and why the street's, the highway's, the building site's and the Technodrome's sections hold. `tests/console/boss_rush.txt`.

- **Sebas's tries of BOSS RUSH and WIDE** (06-11): the crossing ends at the boss's fight's music (0x64, 0x6A), not at
  the 0xFE that stops the music as he comes (Stockman waits there for the last enemies: "me aparecieron ninjas
  antes"), the soldiers kept off through his entrance; the highway (area 5), which has no boss, over as its play
  starts (area_cleared, as the scenarios end area 9), the skateboards (6), with none either, crossed to their own
  end; on the crossing's black the area and its boss's name (ROCKSTEADY, BEBOP, BAXTER STOCKMAN, BEBOP AND
  ROCKSTEADY, ROCK SOLDIER for the building site's and the Technodrome's, which the decompilation names by what
  they are, KRANG AND SHREDDER: Sebas, "el nombre del enemigo"). WIDE in the area's last section (last_segment: its
  boss's) goes no further than the scroll there (x_min, x_max), which the arcade's screen never passes: the first
  boss's apartment and Krang's room are maps of 768 whose ring holds the 512 the game drew, and past the room's
  wall the picture showed the ring's other end, the room again ("hay una fina linea", "se sigue yendo de margen");
  the sides pan there by 4 pixels a frame rather than jump. And the columns a layer's ring does not hold that the
  wider screen shows are drawn from its map at each frame's end (drawn_first, drawn_last). The headless's BMPs'
  rows padded to 4 bytes (a picture of 442 columns).

- **WIDE's checks gone over** (06-11, Sebas: "revisalo"): the skateboards' end (scene 5) is the van's fall drawn
  into layer A by the game, black around it as in the arcade (its columns 30 to 285 in both widths); the
  Technodrome's lift's layer A columns that --wide-check counts are never drawn by the game and show nothing. But its
  shaft (layer B on the diagonal, round and round through the ride) has black past its band, which the arcade's
  screen never shows: its blocks past the band are black (attribute 14 tile 2D below it, 04 F8 above), below it
  in the left side's bottom corner over and over, up to 75 pixels in, and above it at the shaft's end, in the right
  side's top as the lift slows ("corrélo", "arregla lo del pozo, no me lo dejes si lo viste"). Through the ride the
  sides go no further than the first black tile the shaft's map has past the board, where the scroll is and where it
  will be in the next 80 frames (lift_room: the map read along its way, its loop counted), the rest at the other
  side, and what no side can take black at both (in 20:9 the band is narrower than the picture: some 36 at each
  side); sides and bars move by WIDE_PAN. Checked every 4 frames of the ride in both widths: nothing black past the
  bars but the game's own (its rolling balls, the machines' slots). The shaft is no map that loops (its one loop
  through the ride is a jump), so its columns past the board are drawn from the map at each frame; and the columns
  the console has drawn are known only at the layer's height they were drawn at (drawn_y: past the screen's, a layer
  scrolled up or down keeps its older rows there; the left side, open again at the ride's end, showed one). And the
  boss's room after the lift showed past its wall in 16:9: layer A, 2560 pixels before B there, carries B's scroll
  limits, which the last section's rule (and the turtles' room) now take only from a layer inside them.
- **Sebas's tries** (06-11): area 7's girders (layer A, a map of 512 that repeats as the scroll goes) were taken
  for a room, and the picture went all to the left from x 198 on ("en el 8 se movio mucho la barra"): a map no wider
  than the ring repeats itself again, no room nor window (the first boss's apartment, where it started, is a map of
  1536, held by the last section's rule). OPTIONS' version, at the picture's right edge, was cut by 16:9's window,
  which shows the middle 398 of its 416 ("en options esta mal la version"): it is 9 columns in there.
- **BOSS RUSH to each boss** (Sebas: "el boss rush de bebop arranca muy atras", "yo solo quiero jefe", "como la
  zona de krang y como hiciste con rocksteady"): what held the crossing, area by area. The street (2): its scroll
  segment at 1304 ends at a height (768 to 384), and the turtles walked right only: they walk down or up there, by
  the segment's end (crossing_way). The building site (7): the drill's Mousers (off the screen, which their think
  takes as the soldiers' does) and drones (destroyed as by a last hit, flying_drone_struck). The Technodrome (8):
  its turrets on their rails (turret_struck), Rodney (off the screen), the freeze nozzles (freeze_nozzle_struck);
  a wall at a turtle's right, which it walks along, down, then up (its contacts' 08); on the lift the turtles stand
  still, and the ride starts on the players' diagonal (x and y summing 0xC8100: the crossing's walk along the top
  had left it two pixels short, and at the ride's end y went under the map's first row, the game stopping there).
  The final area (9) is Krang's fight from its start (his music, 0x62): no crossing. Each area from 1 to 9 tried
  from its start: the crossing ends at the boss's music in 1, 2, 3, 4, 7 and 8, the highway (5) is over at once.
  The skateboards' road (6), with no boss, is over as its play starts, as the highway. Crossing it the game had
  stopped at frame 5430 reading its code at 04EFF0 as data: enemy_think's second player through a stale pointer
  (with one player the compare's +4 goes through the address the dispatcher called the handler at,
  enemy_skateboard_think's, 04EFEC), the decompilation's 048A98 again with another handler; and the Mousers' at
  0510F8 had stopped area 7 when the road was skipped before. The arcade's too (TMNT Arcade with one player could
  stop there): fixed in the decompilation (Sebas: "sí, arreglalo"), its data_assets.py's CODE_READ_AS_DATA with the
  fifth byte of each handler the dispatcher sends there (the skateboard soldier's 8A, the Mousers', Rodney's and
  Shredder's 2F): its 18 scenarios identical, its assets the ROM's byte for byte; with them the road crossed, then
  skipped, area 7 crossed to its boss, no stop.
- **Sebas's tries of BOSS RUSH** (06-11): with WIDE the street's crossing ended at x 1272, the rest of the street and
  its 18 enemies left to play ("lo de bebop no lo arreglaste, empieza muy atras"): in WIDE's right side the turtle
  came to the turn's wall before the scroll came to the turn, and against a wall the game pushes no scroll. Through
  the crossing, out of sight, the turtles keep the arcade's limits (console_wide_turtle_room). Every area tried from
  its start in the three widths: the crossing ends at its boss's music. And crossing_way read the area's scroll
  segment through its set-up too, when scroll_segment is still the last area's, past this one's list's end (from
  Bebop's area to the next the game stopped reading a record there): only in its play now. F10, the next boss at
  once, for trying the mode (Sebas: "con un F se pase de jefe", "es para probar"): the area over as the scenarios
  end it, Krang's room too (its exit, the Technodrome blown up, the epilogue, the staff and the menu). Taken out
  once tried ("podes sacar el f10"): an area ended with its enemies alive, and the exit, which runs no objects but
  draws them, showed them standing, in its picture's colours (the villains' picture: Sebas, "se piso la capa").
- **Sebas's tries of BOSS RUSH and WIDE** (06-11): BEBOP AND ROCKSTEADY, wider than the screen in the big letters,
  was written in the small ones ("el tamaño del texto del jefe 5 ... es distinto"): in two lines of the big ones now,
  broken at its space nearest its middle. Krang's room has nothing to cross, and its set-up and title (which BOSS
  RUSH's game skips) took 12 frames: his name went by unseen ("krang no lo anuncia"). The console's area_setup_done
  waits on the set-up's last step 150 frames there, his name on the black. An area's exit's picture (exit_cutscene
  set: April carried off, the villains with April, April's hug, the skateboards' crash, Splinter's thanks, the turtles'
  drop before the Technodrome) is drawn into the layers from maps of its own, the layers' records left with the
  area's map: WIDE drew the area's columns past the board's over the picture's, and took its room from them (April's
  hug all at the left, the car street's TCRI building at its right: "se rompio la animacion"). Through the picture
  WIDE takes nothing from the layers; its sides are its background's colour where its edges are all of one
  ("pintemoslo del fondo de color": April's pink, Splinter's yellow, the black about April carried off), black where
  they are not (the villains' octagon, the Technodrome), and the picture at the window's edge where what it shows
  goes on past it (April's turtle's arm, at the left: the pink all at the right). The place is read from the last
  frame drawn: at the picture's first frame it shows in the middle, on the black before it.

Left for later: save states (15), "Original turtles" as a mode of its own (18: in KONAMI CODE meanwhile); a turtle chosen by a player who comes in during a game (today the first
slot free); Kenney Mini still loaded and nothing drawing with it.

## The LAN game (guideline 20): the design (06-11-2026)

Sebas: "avancemos con el proyecto de multiplayer". The library is **arcade-lan** (D:\vs-cheviman\arcade-lan, a
repository of its own for Sebas to create on GitHub, taken by the ports as a submodule as tools/core is
arcade-decomp), in C, with no game in it:

- **The devices.** Up to four in a room, each with its local players (one or more), the room's players four at
  most. Every datagram is UDP, written byte by byte (devices need not be alike: a computer, a phone, a Switch); its
  sockets those of outrunners-port's net_udp.c (Winsock, POSIX, libnx). A device is a handle (lan_open), so that a
  test runs several in one program.
- **The rooms**, as outrunners-port's: a host sends its beacon to the network every half second (and to 127.0.0.1,
  for a second game on the same computer); JOIN lists the rooms heard, forgotten after 3 seconds unheard; a device
  joins by asking until the host's list has it; the host sends the list (its devices in order, the host first),
  and a device's players take the slots in that order. A device unheard for a third of a second is waited for, and
  after 3 seconds it is gone.
- **The game: lockstep through the host**, as the guideline says (one cabinet, as the arcade's TMNT is). The host's
  START sends the game's settings (bytes the game writes and reads: its mode, DIP switches, WIDE...) to every
  device, and frame 0 begins for all. At each of its frames a device hands its local players' controls (a few
  bytes each), which play `delay` frames later (2-3 on a LAN, so that the datagrams have come by then); it sends
  them to the host, the host puts every slot's together and sends every device the frame's inputs of all the
  players; a device runs a frame only once it has them, else it holds (the game shows its last picture). Each
  datagram carries the last frames again, so that a lost one costs nothing. A device gone: its slots' inputs are
  none from a frame the host says, and the game goes on; the host gone: the game ends.
- **Out of step.** Each device hands the state's hash of its frames (tmnt_state_hash: work RAM, palette, the video
  chips' memories); the host compares them with its own and says so to every device when one differs.
- **What a port does**: opens a device with the game's four letters and version, draws its own menus (HOST, JOIN,
  the rooms, the room's list, a player's NAME), gives its controls each frame and runs the frames with the inputs it
  hands back.

**Done in TMNT (06-11, Sebas: the LAN in the root menu).** github.com/cheviman/arcade-lan, the port's `lan/`
submodule (CMake's ARCADE_LAN_DIR; without it the build has no LAN and the page says NO NETWORK); console/lan.c:

- The menu: ARCADE, SPLINTER WAY, **LAN**, OPTIONS, EXIT. LAN's pages over BEST 10 PLAYERS' picture, as OPTIONS':
  HOST, JOIN, NAME (letter by letter, kept in tmnt.ini; HOST or JOIN with none yet ask it first, YOUR NAME, then go
  on: Sebas, "que pueda poner nombre el usuario que se conecta"), BACK, and what went wrong last under them; JOIN the rooms
  found (the host's name, its players "1 OF 4", PLAYING); the room its devices (their player, HOST, YOU, WAITING
  when unheard) and the host's START (two devices or more) and LEAVE.
- The game's start: the host's START sends its DIP switches, region and WIDE; on every device the port stops the
  program (program_stop), resets the board (game_reset) and runs the program again from its start, and the console
  starts as at power-on (console_power_on: every file's own state, wide.c's two statics of functions moved to the
  file) with those settings, no presentation, Splinter Way's turtle choice open from the boot on (under black, LAN
  GAME). The region the host's as the choice opens (show_step). This device's settings back as its game ends, never
  saved over by the host's.
- Each frame: the device's player 1's controls and START (newly pressed) to the network (2 bytes, 3 frames' delay),
  every player's back through console_controls: the room's devices in its list's order are the console's players 1-4,
  each on the turtle he picks (the host's the first). The frame held till they come (the window shows its last
  picture, WAITING FOR its name after a third of a second); the state's hash after each frame to the host.
- No pause: Escape (a game controller's Back) leaves the LAN game, which goes on for the others (the device gone
  after 3 seconds, its turtle standing). The game over, the host gone or OUT OF STEP: the LAN page again, after the
  game over the pause's QUIT GAME has (console_back_to_lan_page: the board's controls and the network's presses let
  go: the headless read the network's back as its players' and its JOIN was taken for HOST).
- tmnt_console_headless: --lan-name, --lan-hashes (each LAN frame's hash), --pace (60 frames a second: the library's
  waits are frames), "quit" leaving a LAN game, its frames counted across the reset (the actions', the dumps').
  Tried with two of them on this computer: a host and a guest that finds the room by its beacon, comes in with START
  on the turtle choice and picks Donatello, the host Leonardo: 2297 LAN frames, every one's hash the same on both, the
  pictures the same pixel for pixel; with the host in 16:9 and the guest in 20:9 and through OPTIONS first, 2197 frames
  the same (the game in the host's 416 columns on both); the guest leaving at frame 2800, the host's game going on.

To come: a device's second player (local players: the library takes them, the menu has no PLAYERS yet), the
window's own try on two computers of a LAN, and the other platforms when the ports come (the library's sockets
are outrunners-port's: Winsock, POSIX, libnx).

## The devices' menus and pause (06-11-2026: Sebas tried TMNT on his phone)

- "la version de android no tiene video, no le puedo poner filtros", "ni fps ni nada": OPTIONS has VIDEO on the
  devices too (TMNT_HANDHELD), with PIXELS and FPS alone: the screen is the window, WIDE as it is (the Switch's 16:9,
  the phone's by its shape, the Anbernic's none).
- "no anda el start para la pausa": START of a player whose turtle plays (or comes in) pauses the game, on every
  platform (console_start_pauses; the port takes that START out of the ones it gives the console). In the pause the
  touch screen's controls work (the stick's up and down, ATTACK the line, JUMP or START go on) and Android's back
  goes on too.
- "el attack queda muy grande en el boton": the touch buttons' names at a scale that fits ATTACK in eight tenths of
  the button (JUMP the same).

## 1.0.1 (06-11-2026: the players' reports on Reddit)

- The players' boxes in WIDE ("the character health indicators would not stay locked to center"): at an area's start
  WIDE's sides are all at the right, and the boxes (the fixed layer's first 32 rows on the screen) moved with the
  board's 320 columns. The video draws that band of the fixed layer where the console says
  (tmnt_video_set_fixed_band, runtime/src/video.c: the port's, as all of WIDE), and wide.c keeps it in the middle
  of the picture: (right - left) / 2 columns further right. The 18 scenarios identical (no WIDE there).
- A game controller before a TV's remote (a Shield's, which Android reports as a game controller and comes first:
  the player's Xbox controller played player 2): ports/sdl2/main.c's controller_added.
- Android's window full screen (SDL's immersive mode: TMNT Arcade's opened as a window, Android 15 drawing it under
  the system's bars), and a device's settings always full screen.
- WIDE 16:10 (a player's request, for the Steam Deck's 1280 x 800): WIDE_16_10, 32 columns more at each side (frames
  of 384), the window's middle 358 (224 x 16 / 10; the menus' crop 13 at each side, wide_crops); after the others in
  the enum (tmnt.ini keeps WIDE by name, a LAN game sends its number: an older device takes it as OFF), second in
  VIDEO's order (wide_order). Android: 20:9, 16:9, 16:10 down to 3:2, OFF.
