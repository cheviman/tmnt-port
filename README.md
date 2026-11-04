# Teenage Mutant Ninja Turtles — decompilation

A reconstruction of Konami's 1989 arcade game *Teenage Mutant Ninja Turtles* (a 68000 main board with a Z80
sound board) into platform-agnostic C, not an emulator, following the method of
[segasonic-decomp](https://github.com/cheviman/segasonic-decomp) and
[outrunners-decomp](https://github.com/cheviman/outrunners-decomp): the game's program decompiled one
function at a time, checked frame by frame against MAME in picture and sound.

**Work in progress.** This repository will contain no game data. As with
[n64decomp/sm64](https://github.com/n64decomp/sm64), you bring your own ROM set (MAME's `tmnt`), and the
tools verify it, extract the assets and build your own executable.

## Status

- [x] ROM set verification (MAME set `tmnt`, World 4 Players, version X), by CRC32 and SHA-1 of every file
- [x] The board model in C (`runtime/`): the 68000's memory map, the inputs and DIP switches, IRQ5 on VBLANK,
      the K052109's and K051960's memories and registers, and their ROMs read through them
- [x] Reference run of the original program on MAME's 68000 core against that model (`kref`): it boots,
      passes its RAM and ROM check, runs the attract mode, and in the test mode passes its mask ROM check
      (the tile and sprite ROMs read through the video chips); the same frame by frame from one run to the
      next
- [x] The reference's picture: MAME's own tilemap, drawgfx and palette code with its K052109 and K051960
      drawing every frame (`kref --video-hashes`, `--video-dump`), as MAME's `tmnt.cpp` draws them
- [x] The reference's sound: the sound board on the board model (`runtime/src/sound.c`), with MAME's Z80,
      K007232 and uPD7759, ymfm's YM2151 and the title music (`kref --sound-hashes`, `--sound-wav`,
      `--sound-trace`): each source's samples at its own rate, the same from one run to the next
- [x] Scenarios (`tests/controls/`, written by `tools/make_scenarios.py`, the format in
      `runtime/include/tmnt/script.h`): the attract mode; the test mode through every screen, every input
      pressed in its I/O test; its sound test through all 135 codes; games of one player (to GAME OVER, and
      one that continues) and of four; one with the DIP switches changed (the screen flipped among them);
      and a four-player game in each of the program's ten areas, started there by a poke and with the lives
      kept up, played until the boss falls and the next area starts (in the street the players walk down and
      up too, or a soldier below them holds the scroll); in the last area they beat Krang, and as Shredder
      and his copies outlast them the area is cleared by a poke during his fight: the ending, the staff
      roll, GAME OVER and the attract mode run. All 17 run to their end (507,382 frames), and between them
      they execute 25,328 addresses of the 68000 program and 4,486 of the Z80's (`python
      tools/check_scenarios.py --reference-only --video --sound`)
- [x] Every boss and the ending in scenarios (04-11-2026): playing them found a picture whose last 14 bytes
      had been taken for a routine (Splinter's, drawn after the building site), which the game read as zeroes;
      a record of the program's data the C reads where its assets have nothing now stops the game, as a byte
      does
- [x] 68000 frontend in the shared core (`tools/core/m68000/`): decoder and disassembler ported from MAME's,
      the same text as MAME's at every even offset of the program and of an image of every opcode
      (`python tools/core/konami/reference/compare_m68k_with_mame.py`), with each instruction's operands
      and control flow
- [x] Recursive disassembly without traces (`python tools/disassemble.py`, the core's 68000 analysis, from
      the vectors and `symbols.txt`): it follows what the address registers can hold into the tables of
      object handlers the program jumps through, and finds 27,892 instructions, all 25,328 addresses the
      scenarios execute among them and none of them inside another instruction
- [x] Every byte of the program classified (`python tools/coverage.py --reads build/scenarios`, the core's,
      with the bytes the scenarios read as data: `kref --rom-reads`): 32.4% code, 65.8% data,
      1.6% fill, and 236 bytes (0.06%) not explained yet
- [x] The data the engines read, walked from where the code hands it over (`python tools/data_walk.py
      --reads build/scenarios`, no traces): the sprite engine's animations, sprite lists and boxes, the
      areas' layer maps and blocks, the palettes and their effects, the enemies' spawn lists and
      definitions, the texts, the scroll's segments, the attract mode's demos, and the tables the code reads
      by an index or through a register
- [ ] Every table of data described with its layout, its types and a name by subject, before any C is
      generated: the C reads the game's data by name from its first build, never by an address of the ROM
      (today 236 bytes are unexplained, and 426 bytes the scenarios read are in no table described yet)
- [x] The sound program classified the same way (`python tools/sound_coverage.py --trace build/scenarios
      --reads build/scenarios`): its analysis finds the 4,481 addresses the scenarios run without traces, and
      every byte is code (35.6%), described data (64.3%: the driver's tables and the music) or fill
- [x] Assets extracted (`python tools/extract_assets.py`, into `assets/`, outside the repository) and checked
      byte for byte against the ROM set (`--check`): tilesets by area and sprite sheets by the animations
      that show them (indexed PNGs that keep each pen), the title music, the K007232's samples and the
      uPD7759's speech (WAV), and the data of both programs as text by subject, every piece by name
- [ ] The assets by theme throughout: tiles no area uses yet are in sheets by bank, the music is text (not
      MIDI yet), and many pieces are named by their kind and place
- [x] The 68000's instructions lifted to the decompiler's IR (`tools/core/m68000/lift.py`), checked
      against MAME's 68000 core one instruction at a time from random states (`python
      tools/core/konami/reference/compare_lift_with_mame.py --c`): every opcode of the program and every
      valid opcode of the 68000 leave the same registers, flags and bus cycles, in the IR and in the C
- [x] Translation scaffold (`python tools/recompile.py`): the program as C an instruction at a time, run by
      the board in C; the game's own video (`runtime/src/video.c`) and sound chips (`runtime/src/ym2151.c`,
      `sound_chips.c`) match MAME's frame for frame and sample for sample (`kref --video-check`,
      `--sound-check`)
- [x] The game runs from its assets (`runtime/src/assets.c`, `kref --assets-check`: every byte comes back),
      opening no ROM: all 17 scenarios match the reference frame by frame in state and picture
      (`python tools/check_scenarios.py --native-only --translated --video`), the window port (`tmnt`,
      SDL2) gives the same frames from a script, and no executable of the game holds a stretch of the ROM
      set (`python tools/rom_in_executables.py`)
- [x] The sound program decompiled (`python tools/decompile_sound.py`, into `src/sound/`: 381 functions, no
      goto, its tables read by name from the assets) and run by the game's own sound board
      (`runtime/src/sound_cpu.c`, `sound_board.c`) on the Z80's timing model, which the reference follows
      too: its samples and RAM match MAME's sound board in every frame of the 17 scenarios (`python
      tools/check_sound_board.py`, 241,282 frames), and the game matches the reference in state, picture
      and sound in all 17 (`python tools/check_scenarios.py --translated --video --sound`), in the window
      port too
- [x] The 68000 program decompiled (`python tools/decompile.py`, the core's 68000 driver, into `src/`): one C
      function for each of the program's 2,059, no goto, the stack, the return addresses and the status
      register's flags gone into the C's own calls and variables; the tables it reads by name (393) loaded
      from the assets, the data it reaches through the addresses it keeps resolved through the assets, never
      the ROM. The game runs it on its own runtime (`runtime/src/game_program.c`: the frame, the interrupts,
      the tables), and matches the reference in state, picture and sound in all 17 scenarios (`python
      tools/check_scenarios.py --native-only --video --sound`), in the window port too; the structure of
      every function, the code no scenario runs included, checks against its jumps (`python
      tools/check_structure.py build/structure_check`: 0 problems), and no executable holds a stretch of the
      ROM set. The work RAM the comparisons leave out is what the 68000 itself keeps there and the C has in
      its variables: the two stacks (060700-060AFF) and the registers the VBLANK handler saves (060440-06047B)
- [x] The C's types (30-10-2026): the 50 objects (0x50 bytes from 062000) are a structure the C reads by its
      fields (`OBJECT(object)->frame`), the program's data the sprite engine walks are records with fields
      (`ANIMATION_FRAME(frame)->sprites`, `SPRITE(sprite)->x`, the attack frames, sprite sizes and body boxes),
      read from the assets, never the ROM; the longs the 68000 writes a half at a time go to the work RAM as
      one value. Memory reached through an address the code works out went from 12,127 to 2,228 (the rest:
      buffers, the video chips' memories and tables the code walks byte by byte)
- [x] The objects named (30-10-2026): the fields every object has (its attack and the hit it took, health,
      damage, speeds, floor, depth), and families of objects by the code that runs them, each with the fields
      as its code uses them (`OBJECT(turtle)->turtle.jump_speed`, `OBJECT(enemy)->enemy.target`): the
      turtles, the enemies (the Foot soldiers by weapon, Rocksteady, Bebop, the Mousers, Roadkill Rodney,
      Shredder) with a family for each kind of move that keeps something else in its fields, their
      projectiles, the doors, lifts and manholes they come out of, the scenery that hits. A family whose name
      starts with another's reads that one's fields too (`enemy_grab` is an `enemy`). The uses of the
      objects' fields named after their offset went from 5,618 to 1,782, the functions named after their
      address from 2,212 to 1,197
- [x] The other objects named (30-10-2026): Krang's android body with his fist, his lasers and the curtain he
      appears behind, Baxter Stockman and the Mousers he drops, the two rock soldier bosses of the building
      site and the Technodrome with their shots, bars and rubble, the helicopters of the skateboard ride, the
      flying drones, the parked car of the street, the areas' scenery and traps (hydrants, signs, rats, an
      alligator, pizza, oil drums, turrets, freezing nozzles, electric beams, spiked poles, the sewer's
      missiles) and the explosions, each kind with its family and fields, every function with a comment, in
      files by subject (`src/objects/krang.c`, `stockman.c`, `rock_soldiers.c`, `air_attacks.c`, `scenery.c`,
      `explosions.c`, `helpers.c`); an unused colour editor for the palette found among them
      (`src/system/palette_editor.c`). The uses of the objects' fields named after their offset went from 1,782
      to 1,124, the functions named after their address from 1,197 to 795
- [x] Every function of the 68000 program named (30-10-2026), each with a comment, and every file by subject
      (`src/unsorted/` is gone): the boot and its RAM and ROM check, the frame's interrupt, the coins and the
      players' lives, boxes and messages, the jobs the frames queue for the main loop, the texts, the test mode's
      pages (an unused coin counter test among them), the attract mode (the opening, the title, the coin's
      screen, the profiles, BEST 10 PLAYERS, the demos), the game's scenes (an area's story and title, the play,
      the area's exits with their cutscenes, the continue, the ranking with its name entry, the ending with its
      staff roll), the layers' maps and the scroll held while too many enemies are on screen, the speech
      balloons, the palettes' requests, fades and effects, and the object engine (`src/scenes/`, `src/stage/`,
      `src/video/`, `src/system/`, `src/objects/turtles.c`). The functions named after their address went from
      795 to 230: the sound program's 225 and five that are a lone return nothing tells apart from its
      neighbours
- [x] The work RAM named (30-10-2026): 251 variables, and the records the program keeps one after another as
      structures (`PLAYER(i)->lives`, the coin slots, the palette effects and requests, the three layers with
      the working one and layers A and B by name, the typed texts); the objects' stats are records of the
      program's data read by their fields. The uses of variables named after their address went from 3,510 to
      549, the memory reached through an address the code works out from 2,185 to 1,735
- [x] The 68000 program's data read by name (31-10-2026): every table it reads has a name and a type
      (`krang_animations[3]`, `turtle_throw_moves[i]`, the tables of records `ending_palette_fades[2].banks`),
      every record it reaches through a pointer a structure (the enemies' definitions and attacks, the arcs of
      their jumps, the shots and the objects one object starts, the scroll segments, the spawn records), the
      bytes a pointer walks one at a time read by what they are (`TEXT(p)`, `BLOCK_BYTE(p)`, `MAP_ENTRY(p)`),
      and every address of the program the C holds as a value by its name (217 animations, the shadows, the
      tile blocks, the layers' pictures); a constant stored into a position or compared with one is a number,
      not an address. No read of the 68000 program's C goes through an address it works out (248 before), no
      table is named after its address (276 uses before), no address of the program is written as a number
      (378 before)
- [x] The sound program's data read by name too (31-10-2026): the tables it reads through HL by their names
      (`fm_effect_handlers`, `fm_voice_pointers[i]`, `pcm_note_pitches`, those it indexes from before their
      start, `TABLE16(fm_note_pitches, i - 2u)`), the instruments' and drums' records by their fields
      (`PCM_DRUM(pcm_sound)->pitch`), the track commands' handlers by what they are: no read of the whole C
      goes through an address any more (27 in the sound program before)
- [x] The 68000 program's C named (01-11-2026): every function with a name and a comment (the jobs 7, 8 and 0xA
      and the enemies' attacks 6 and 7 are lone returns named for what they are and where they sit), every variable
      of the work RAM (the 12 nothing reads are `unused_...`), every field of the objects, and the parameters and
      locals by what they hold: the object a handler runs on is `object`, a player's slot `slot`, a value stored into
      a field or compared with one takes its name, a loop's counter is `count`, and what a function only hands on
      takes the name the functions it calls give it (the decompiler works these out, `tools/game_profile.py`; about
      2,400 more are named in `symbols.txt`, each with the instruction that shows it). The pointers into the
      program's data are typed (the texts, the tile blocks, the palettes' colours, the fire's records, the demos'
      recorded inputs, the turtles' walking speeds...), and the tables the C still read through an address it
      wrote as a number (`mem_read32(0x53D2Au + move)`) are read by name (`TABLE32(enemy_attacks, move)`). In the
      68000's C: parameters and locals named after a register 7,297 → 2,299, variables after their address 16 → 0,
      fields after their offset 106 → 0, functions after their address 5 → 0, functions without a comment 713 → 0,
      memory reached through an address the code works out 1,437 → 1,123; still 17 of 17 scenarios identical in
      state, picture and sound
- [x] The sound program's C named (01-11-2026): every function with a name and a comment (each effect's handlers
      `fm_effect_XX_...` and `pcm_effect_XX_...`, the routines the FM effects share: `fm_effect_fall`,
      `fm_effect_trill_slide`, `fm_effect_set_pitch`...), the channel records by what plays on each
      (`CHANNEL(SPEECH)->sound`, `CHANNEL(MUSIC_FM_6)`), their fields as the music uses them (`track`, `ticks_left`,
      `gate`, `note_hook`, `loop_start`...) and as the FM and PCM effects use the same bytes (`->fm_effect.timer`,
      `->pcm_effect.ticks`, and an effect's own where it differs), every variable of the RAM, and the parameters and
      locals by what they hold (the record a handler runs on is `channel`; the rest by the rules the 68000's C uses,
      two more of the sound program's own, and `sound_symbols.txt`, each name with the instruction that shows it). In the
      sound program's C: functions named after their address 225 → 0, variables after their address 533 → 0, fields
      after their offset 912 → 0, parameters and locals after a register 1,039 → 0, functions without a comment
      381 → 0; still 17 of 17 scenarios identical in state, picture and sound
- [x] What the 68000's routines leave in registers for the next one, part by part (01-11-2026): the decompiler
      now knows what a function reads to hand back each part of a register (byte, byte, high word), not the whole
      register, and that "bset #7, D7" or an or/and with a constant writes only the parts it changes; what a function
      hands back is what its callers read once everything is known (it had kept what they seemed to read while the
      summaries grew: `queue_sound` took A6 for no caller); four enemies' tables of states were read past their end
      into the arcs after them, two phantom states each that made every state handler read D7. A function hands back
      only the parts some routine reads before writing them again, and the values a function works out itself have
      names (the facing bits, the walks' animation tables, the speeds, the damages, the records an enemy throws, the
      stick's directions, the fill routines' zeroes). In the 68000's C: parameters and locals named after a register
      2,299 → 1,599, functions with a parameter named after one 910 → 639, faithful functions 57.0 % → 67.9 %, partial
      writes that put a part of one value into another 321 → 185; still 17 of 17 scenarios identical in state, picture
      and sound
- [x] What the frame carries in registers, as global variables (01-11-2026): what one routine leaves in a part of a
      register and a later one reads before setting it, all through the frame from the VBLANK handler down, is the
      game's state, and the C keeps it in six global variables named for what their readers take them as
      (`offset_high_word`, D7's high word, which `hydrant_spray`, `billboard_wait` and `rock_soldier_site_fire` add to
      a position after `enemy_landed` left FFFF there; `distance_low_byte`, D5's low byte, which `helicopter_fly`
      reads as its in-line flag when the first turtle it finds is past x 0x1FF; `palette_place`, the A5 of the scenes
      and the palette requests; `screen_x_high`, `distance_high_word`, `fade_banks`). What a function works out and
      uses itself stays a local, copied into the global where it goes on. The runtime puts them back when an
      interrupt's handler returns, as the VBLANK handler puts back the registers it saves. A function that hands back
      a value from a switch whose every case returns says its end is never reached (`unreachable()`): the C builds
      without a warning. In the 68000's C: parameters and locals named after a register 1,599 → 478, functions with a
      parameter named after one 639 → 350, faithful functions 67.9 % → 77.9 % (lines 98.2 %), results written back
      before an early return 66 → 3, partial writes that put a part of one value into another 185 → 83; still 17 of
      17 scenarios identical in state, picture and sound
- [x] Nothing named after an address or a register, and 99 % of the C faithful (04-11-2026): what the object loop
      hands each object's routine in A5 and A1 is kept as the game's state (`object_loop_leftover`,
      `object_loop_slot`); the tables of states the objects run (`enemy_hop_states`), the switches' tables of handlers,
      the routines nothing reaches (`say_speedy`, `word_to_decimal_digits`) and every piece of data the C named after
      its address have names, and the switches on a handler name their cases after it (`case enemy_hop_crouch_ADDRESS:`).
      The pointers the C reached memory through by address are typed: a players' record by the player's number
      (`PLAYER(PLAYER_ADDRESS(0) + (int16_t)offset)->lives`), an array of the work RAM by a signed word, a place in the
      tile RAM plus one, the enemies' reactions by the side they were hit on (`REACTION(reactions + side)`), the object
      that hit a turtle (`OBJECT(hit_by)->hit_sound`), the fields an object's family keeps pointers in. A byte written
      into a register whose other bytes are read afterwards is that variable changed in place, a statement of its own
      (`move_offset = (move_offset & ~0xFFu) | moves[move_offset];`). The routines that clear or copy memory a long at
      a time (the boot, its RAM tests, the turtles' objects, the layers' copies, the ranking's defaults) are memory
      routines, each with its reason (`tools/game_profile.py`). In the sound program, the borrow of `sbc hl,de` is the
      comparison it is (`offset + carry > 0xA00u`) and its handlers are named. In the whole C: functions with nothing
      but the faithful leftovers 80.7 % → 99.3 % (lines 99.9 %), parameters and locals named after a register 478 → 0,
      names made of an address 421 → 0, memory reached through an address the code works out 1,114 → 468 (the video
      chips' memories, read by the bus as the board has them, and the memory routines), partial writes that put a part
      of one value into another 83 → 3, flags worked out as the CPU sets them 8 → 0; still 17 of 17 scenarios identical
      in state, picture and sound
- [ ] What is left of the structure: 16 functions of 2,435, each for a reason: 8 keep a control flag their structure
      needs, the enemies' retargeting reads a second slot that may be none through the bus (as the original, in 4), the
      demo's input channels read past a player's record, the name entry adds one to an initial through an address the
      instruction works out, Rocksteady's moves index a table with a word whose low byte comes from the data, and code
      nothing runs (the palette editor keeps its stick at address 000006, in the program; a list of palette requests
      whose place is not known; two routines with a stack frame the C has not; one that writes its results before
      each of its two returns). In the sound program, the 16-bit fields it writes a byte at a time show their two bytes

## Getting started

Requirements: Python 3.10 or newer, and git (and for the reference tools, CMake and a compiler: below).

```bash
git clone --recursive https://github.com/cheviman/tmntarcade-decomp
```

The decompiler and the analysis are in [arcade-decomp](https://github.com/cheviman/arcade-decomp), which this
repository keeps in `tools/core` as a git submodule (a clone made without `--recursive` takes it with
`git submodule update --init`).

Put your own copy of MAME's `tmnt` set in `rom/` (the .zip as is, or its files), and check it:

```bash
python tools/verify_rom.py
```

| File | Size | CRC32 | What it holds |
|---|---|---|---|
| `963-x23.j17`, `963-x24.k17` | 128 KB each | `a9549004`, `e5cc9067` | 68000 program, 0x00000-0x3FFFF (even, odd bytes) |
| `963-x21.j15`, `963-x22.k15` | 64 KB each | `5789cf92`, `0a74e277` | 68000 program, 0x40000-0x5FFFF (even, odd bytes) |
| `963e20.g13` | 32 KB | `1692a6d6` | Z80 sound program |
| `963a28.h27`, `963a29.k27` | 512 KB each | `db4769a8`, `8069cd2e` | tile graphics (K052109) |
| `963a17.h4`, `963a15.k4`, `963a18.h6`, `963a16.k6` | 512 KB each | `b5239a44`, `1f324eed`, `dd51adef`, `d4bd9984` | sprite graphics (K051960) |
| `963a30.g7`, `963a31.g19` | 256 bytes each | `abd82680`, `f8004a1c` | sprite address decoder, priority PROMs |
| `963a26.c13` | 128 KB | `e2ac3063` | K007232 samples |
| `963a27.d18` | 128 KB | `2dfd674b` | uPD7759 speech |
| `963a25.d5` | 512 KB | `fca078c7` | title music sample |

The game and the reference tools (for working on the decompilation: they run the original program on MAME's
cores) need CMake 3.20 or newer and a C11 and C++20 compiler (Visual Studio 2026 on Windows). The game's data
is its assets, which the extractor writes from your ROM set:

```bash
python tools/extract_assets.py
```


```bash
cmake -S . -B build/msvc
```

```bash
cmake --build build/msvc --config Release --parallel 6
```

The game, with no screen (`tmnt_game_headless`, hashes of every frame) or in a window (`tmnt`, with SDL2):

```bash
build/bin/tmnt_game_headless --script tests/controls/attract.txt --hashes build/attract.txt
```

```bash
build/bin/tmnt
```

```bash
build/bin/kref --frames 3600 --hashes build/kref/hashes.txt --text 3000
```

```bash
build/bin/kref --frames 3600 --video-hashes build/kref/video_hashes.txt --video-dump 1200 2400 --video-dir build/kref
```

```bash
build/bin/kref --frames 3600 --sound-hashes build/kref/sound_hashes.txt --sound-wav build/kref/attract.wav
```

## Layout

```
rom/                your ROM set (not in the repository)
runtime/            the board as the program sees it (tmnt/machine.h), and the reader of the ROM set
tools/core/         arcade-decomp, the shared core (git submodule); its konami/reference/ holds kref
tools/game_profile.py   what the core needs to know of this game
tools/tmnt_rom.py   the ROM set: its files by hash, and the regions as the board lays them out
tools/verify_rom.py the ROM set's check (the core's script)
tools/check_scenarios.py   every control script on the reference, and the static analysis against them
tools/disassemble.py       the static analysis: build/listing/tmnt.s and report.txt (the core's)
tools/coverage.py          every byte of the program as code, data or fill: build/listing/coverage.txt
tools/data_walk.py         the data the engines read, walked from the code: build/listing/data_walk.txt
tools/sound_coverage.py    every byte of the sound program as code, data or fill (the core's)
tools/extract_assets.py    the assets from the ROM set, and --check back (tmnt_graphics, sound_assets,
                           data_assets, sound_data_assets)
tools/decompile.py         the 68000 program as C into src/ (the core's m68000/decompile.py)
tools/decompile_sound.py   the Z80 sound program as C into src/sound/
tools/check_structure.py   every function's ifs, loops and switches run against its jumps
tools/noise_report.py      what the C still has of the CPU and of the ROM's places, against tools/noise_baseline.txt
src/                the decompiled C (generated: change the tools or symbols.txt, not the C)
runtime/src/game_program.c the runtime of the decompiled program: the frame, the interrupts, the tables by name
runtime/replaced.txt       the program's functions the board puts its own in place of (the boot's ROM test)
ports/              the game with no screen (headless, for the checks) and in a window (sdl2)
sound_symbols.txt          the sound program's names, and what its analysis cannot work out alone
symbols.txt         the program's names, and what the static analysis cannot work out alone
tools/make_scenarios.py    writes the control scripts
tests/controls/     the control scripts
```

## Legal

The code in this repository is under the BSD 3-Clause License (see `LICENSE`). It contains no ROM data, no
assets extracted from the ROM, and no code of the original game other than the decompiled C. MAME's
`tmnt.cpp`, `k052109.cpp` and `k051960.cpp` (BSD-3-Clause) are the source of the ROM set's layout and of the
board model's behavior. MAME's and ymfm's own code is in arcade-decomp's reference tools only, under their
licenses.

*Teenage Mutant Ninja Turtles* is a trademark of its owner, and the arcade game is Konami's. This project
is not affiliated with or endorsed by either.
