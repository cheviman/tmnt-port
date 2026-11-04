"""
Name:        sound_music.py
Description: the sound program's music as Standard MIDI Files, a song a file, and back byte for byte
Version:     0.1.0
Created:     04-11-2026
Modified:    04-11-2026
Author:      Sebastian Toyos

For tools/sound_data_assets.py, which writes the music into assets/sound/music: a file a song (sound_symbols.txt's
songs table, the commands 60-78), named by what the game plays it for, and music.txt, the files and their songs.
The game loads them (runtime/src/music.c), and --check rebuilds every byte of the music and the songs table from
them.

How the driver plays a song (src/sound/music.c, sound_symbols.txt):
- start_music (01C4) puts the music's 9 channels (YM2151 channels 1-7, the K007232's A and B) on
  music_track_start: each channel's record cleared from +12, its track the song's entry of songs (9 addresses a
  song); the music's transposition (E8), volume (EA) and accelerando back to 0, its tempo (E1) as it was. The next
  pass reads each track's first commands (music_track_first_tick); from then on music_track_tick: the tempo
  (tempo + tempo_increase, a byte; 0 stops the channel) into the channel's +12 each pass, a tick at each carry,
  the next commands when the note's ticks run out. FM channel 1 counts the accelerando (FC-FE) first: the tempo
  one faster every 16 x its low nibble passes. A pass is the main loop's: it waits for the YM2151's timer B
  (register 12 = F2: 14 x 1024 clocks of 3.579545 MHz), starts it again, then runs the channels FM 1-7, PCM A,
  PCM B in order.
- read_track (0A46) reads commands up to a step, a byte at a time (the lengths are sound_data_walk.py's):
      0x            the octave: x 24 for the YM2151 (the bytes of fm_note_pitches), x 1 for the K007232
      7x            the attenuation step x
      LN            a note: L (1-6, 8-D) its length, note_lengths[L - 1] ticks; N (1-F) its pitch; N 0 a rest
      E5 b1 .. bn   a held note: b1's high nibble a length, then the nibbles of the next bytes, two a byte, up to
                    the first 0 nibble (its byte the last); the pitch is b1's low nibble (bn's on a K007232
                    channel in instrument mode); the ticks add up in a byte
      E0 mode (bit 0: a K007232 channel's drums), E1 tempo, E2 instrument, E3 volume, E4 gate, E6 detune,
      E7 portamento, E8 the music's transposition, E9 the channel's, EA the music's volume, EB the channel's
      volume offset, ED the YM2151's LFO (2 bytes), EE the LFO ramp, EF nothing, FC-FE accelerando: a byte
      each; EC count, transposition, volume offset, address: jumps there with the transposition and volume
      added, count less one times; F0 address: jumps; F1 count, address: jumps there the count-th time; F2/F4
      count: a loop (F3/F5 its end: back count less one times); F6 count, address: plays the part there count
      times (F7 returns); F8, FA address: plays the part (F9, FB return); FF: the end
  A transposition byte is 12 x bits 4-6 plus bits 0-3 semitones, down with bit 7. An FM note's pitch is
  fm_note_pitches at the byte (the music's transposition + 2 N + octave + transpositions) less 2; a K007232
  note's is the drum N of pcm_drums, or pcm_note_pitches at N + 12 x octave (no transposition).

The files (write): Standard MIDI Files, format 1, 24 ticks a quarter note: a file's tick is a driver's tick
(note_lengths: 96 a whole note, 24 a quarter, 6 a sixteenth, 36 a dotted quarter, 16 a triplet quarter...). The
tempo events give the ticks their time, so a player plays the song at the game's speed: a tick is 256 / tempo
passes. A pass starts timer B, which runs out 14 x 1024 clocks later on a grid of 1024 (ymfm starts it short by
the low 4 bits of its sample count, as its divide by 16 runs free), so a pass whose work ends before then takes
14 x 1024 clocks (249.7 a second), whatever the wait for the flag; one whose work goes past it starts the next
late. The notes' work does that often: the boss's music plays at 237.3 passes a second, area 0's at 241.7. The
writer counts each pass's work (PASS_WORK: what its channels read) and gives each stretch of a tempo the time its
passes take (Player.pass_time). Tracks:
  1     the song: its name (FF 03), the 'N' event, the tempo events (FF 51) where the tempo changes (E1 and the
        accelerando, at the tick of the pass where it does: the stretch's time over its ticks), the 'U' events
  2-10  the driver's channels: FM 1-7, PCM A, PCM B, each its name (FF 03), the 'S' event, then what the
        channel reads, as the driver plays it
The driver's events that MIDI has none for are sequencer-specific meta events, FF 7F LEN 7D TAG DATA (7D: the
non-commercial manufacturer id; LEN a variable-length quantity):
  'N' (4E)  the songs table's entries (0-24: the commands 60-78) that play this file's tracks
  'S' (53)  the place the songs table points at for this channel: a name of the assets' (sound_layout.txt), as
            ASCII text, maybe +0xNN
  'C' (43)  a command, its bytes as the driver reads them, an address last as a place's name (ASCII text): the
            octave and attenuation steps (0x, 7x) and E0-FF but E5
  'R' (52)  a rest: no data, its length the time to the channel's next step
  'H' (48)  a held note: the bytes after E5 (the Note On at the same tick plays it)
  'Z' (5A)  where the file stops showing the channel: no data; the step before it ends here
  'U' (55)  bytes no file shows (no track reaches them, or only after the file's end): a place's name, a 0
            byte, the bytes from there
A note is a Note On and a Note Off; its length is the time to the channel's next step (note, rest, held note,
'Z', or the command FF), its key the pitch: an FM note's the key of the YM2151's key code (11 + the byte of
fm_note_pitches / 2: N 1 at octave 0 is key 12), a K007232 note's in instrument mode 11 + N + 12 x octave, a
drum's DRUM_KEYS[N]. At a tick, a channel's commands come before its step, and every channel's events at a tick
come before the next channel's (the music's transposition E8 counts for the channels after it in the pass).
The channels play loops and parts as the driver does: a loop's turns one after the other, a part where it is
called, a jump where it goes; every channel goes on to the tick where the last of them first comes back to a
state it had (or ends, or the tempo stops it), or CAP_PASSES passes; after that tick its next commands, then a
'Z'. A Note Off ends a note where the driver keys it off: an FM note by its gate (E4: the note's ticks less
ticks x gate / 256 + 1, a tick at least), a K007232 note at its length. The writer's own, for players, which the
way back skips: the tempo events, the names, the velocity (the attenuation the note plays with), the MIDI
channel (the YM2151's channels 1-7 on 1-7, the K007232's A on 8 and B on 10, its drums on 9: General MIDI's
percussion), the program changes (FM_BASS below key 48, else FM_LEAD; the K007232's instruments PCM_BASS).

The way back (read): every driver's track, in the order of the file's tracks with an 'S' event, gives its steps:
the events at a tick (commands, then a step), the Note Offs aside; the tracks are read together, by tick and then
by track. Each channel keeps the driver's record (octave, transpositions, mode, loops, returns) and its place,
from 'S': a command's bytes go there (an address as the place its name says) and the place moves on as the
driver's would (a jump, a loop's end, a call, a return); a note's byte is its length's code (note_lengths, the
first that matches; 7 is no note's) and the pitch the key gives from the channel's octave and transpositions
(a held note: the 'H' bytes with b1's pitch from the key, but in instrument mode on the K007232, where the key
has to be the bytes'); a rest's, the length's code. A byte that comes back twice (a loop's later turns, a part
played again, the same place in two files) has to be the same. The 'N' events give the songs table's entries,
the 'U' events their bytes. What does not come back says what failed: a file that is no Standard MIDI File, a
tick that is no whole driver's tick, a length no code has, a key no pitch nibble gives, a Note Off that is not
where the gate puts it, a byte two places give differently, a place no name has, a music byte no file gives.
"""
import math
import re
from bisect import bisect_right
from collections import defaultdict

import sound_data_walk

TICKS_A_QUARTER = 24            # the files' division: a driver's tick a file's tick (96, a whole note)
SOUND_CLOCK = 3579545           # the Z80's and the YM2151's clock (Hz)
TIMER_B_SAMPLES = 16 * (256 - 0xF2)     # restart_frame_timer's timer B (register 12 = F2): samples of 64 clocks
POLL_CYCLES = 205               # the main loop's wait: a turn of the random bytes, then the YM2151's flag
RESTART_CYCLES = 380            # from the flag to timer B's start (restart_frame_timer's three register writes)
# a pass's work, Z80 cycles: its base, and what it reads (the channels' notes and rests, the instruments it loads,
# its commands), as the game's own sound board takes them (runtime/src/sound_cpu.c's timing model; MAME's Z80 in
# kref plays the songs at the same speed): fitted to its passes a second during each song (the title music's pass
# counter, 83C5, read at every frame for 13 x 250 passes from the song's start: 237.3 for the boss's, 249.7 for the
# silence); the model gives each of the 25 songs within 1% (0.4% for the root mean square)
PASS_WORK = {"base": 3900, "fm_note": 4900, "fm_rest": 1000, "pcm_note": 3000, "pcm_rest": 1400,
             "instrument": 3600, "command": 160}
CAP_PASSES = 45000              # a file shows at most some 3 minutes of its song (CAP_PASSES passes)
SONG_TRACKS = 9                 # a song's channels: FM 1-7, PCM A, PCM B
FIRST_SONG = 0x60               # the command of the songs table's first entry
TAG = 0x7D                      # the non-commercial manufacturer id of the meta events FF 7F
FM_BASS, FM_LEAD, PCM_BASS = 38, 62, 33     # General MIDI programs: synth bass 1, synth brass 1, electric bass
DRUM_CHANNEL = 9                # General MIDI's percussion channel (10 to people)
# pcm_drums 1-15 as General MIDI percussion keys: the kick (sample 0), three snares (sample 1 at three pitches),
# the hi-hat (sample 2), ten toms going up (sample 3)
DRUM_KEYS = (None, 36, 38, 40, 37, 42, 41, 43, 45, 47, 48, 50, 64, 63, 61, 60)
CHANNEL_NAMES = ("YM2151 channel 1", "YM2151 channel 2", "YM2151 channel 3", "YM2151 channel 4", "YM2151 channel 5",
                 "YM2151 channel 6", "YM2151 channel 7", "K007232 channel A", "K007232 channel B")

# a command's bytes (E5 aside), and the place of the address the ones that carry it have (always last)
COMMAND_LENGTHS = dict(sound_data_walk.FIXED)
COMMAND_LENGTHS.update({0xEC: 6, 0xF0: 3, 0xF1: 4, 0xF6: 4, 0xF7: 1, 0xF8: 3, 0xF9: 1, 0xFA: 3, 0xFB: 1, 0xFF: 1})
ADDRESS_AT = {0xEC: 4, 0xF0: 1, 0xF1: 2, 0xF6: 2, 0xF8: 1, 0xFA: 1}

# what the game plays each song for (the 68000's code that queues the command), its file's name; the others are
# song_XX, and a song with the same tracks as one before it goes into that one's file
SONGS = {
    0x60: ("area_6", "area 6's (the highway on skateboards): area_music[6]"),
    0x61: ("area_5", "area 5's (the highway): area_music[5]"),
    0x62: ("area_9", "area 9's (Krang and Shredder in the Technodrome): area_music[9]"),
    0x63: ("area_0", "area 0's (the burning building): area_music[0]"),
    0x64: ("boss", "a boss's: start_area_music, the bosses' entrances"),
    0x65: ("area_3", "area 3's (the sewers): area_music[3]"),
    0x66: ("area_4_and_7", "areas 4 and 7 (the street of cars, the building site): area_music[4], area_music[7]"),
    0x67: ("area_2", "area 2's (the street): area_music[2]"),
    0x68: ("area_1", "area 1's (April's apartment): area_music[1]"),
    0x69: ("area_8", "area 8's (the Technodrome): area_music[8]"),
    0x6A: ("shredder_appears", "Shredder steps out: enemy_shredder_appear_step_out"),
    0x6C: ("ending", "the epilogue: ending_fade_to_wreck"),
    0x6D: ("game_over", "GAME OVER: game_over_start, ending_game_over_start"),
    0x6E: ("area_exit", "an area's exit: turtle_exit_sewer_start, turtle_exit_building_site_start, "
                        "exit_car_street_van_start_up, exit_skateboard_van_door"),
    0x6F: ("coin_screen", "the attract's coin screen: coin_screen_draw, coin_screen_wait"),
    0x70: ("staff_roll", "the staff roll: staff_roll_start"),
    0x71: ("ranking", "the name entry of the best players: ranking_choose_entry"),
    0x72: ("shredder_takes_april", "area 1's exit: exit_apartment_shredder_takes_april"),
    0x73: ("area_2_and_8_exit", "the exits of areas 2 and 8: turtle_exit_street_start, turtle_exit_technodrome_start"),
    0x75: ("area_7_exit_drop", "area 7's exit, the turtles drop in: turtle_exit_building_site_drop"),
    0x77: ("continue", "CONTINUE and its count: continue_start"),
    0x78: ("profile", "the attract's profiles: profile_start_text, profile_move_picture"),
}


class MusicError(Exception):
    """What does not come back from the files, or does not go into them"""


# ---- the driver ----

def transposition(byte):
    """read_transpose: 12 x bits 4-6 plus bits 0-3 semitones, in bytes of fm_note_pitches (two a semitone)"""
    value = (24 * ((byte & 0x70) >> 4) + 2 * (byte & 0x0F)) & 0xFF
    return (-value) & 0xFF if byte & 0x80 else value


def attenuation_offset(byte):
    """read_volume_offset: bits 0-6, negative with bit 7"""
    return (-(byte & 0x7F)) & 0xFF if byte & 0x80 else byte


def held_length(data):
    """The bytes of a held note after E5, from 'data' (up to the byte with a 0 nibble, b1 always)"""
    at = 1
    while at < len(data) and data[at] & 0xF0 and data[at] & 0x0F:
        at += 1
    return min(at + 1, len(data))


def length_of(code, lengths):
    """note_lengths' length of a code (1 on); None for one the table has not"""
    return lengths[code - 1] if 1 <= code <= len(lengths) else None


def held_ticks(data, lengths):
    """The ticks of a held note's bytes (after E5), as the driver adds them up (in a byte; 0 is 256); None when a
    code is none of note_lengths'"""
    codes = [data[0] >> 4]
    for byte in data[1:]:
        if not byte & 0xF0:
            break
        codes.append(byte >> 4)
        if not byte & 0x0F:
            break
        codes.append(byte & 0x0F)
    parts = [length_of(code, lengths) for code in codes]
    if None in parts:
        return None
    return (sum(parts) & 0xFF) or 256


class Music:
    """The music's variables, every channel's"""

    def __init__(self):
        self.tempo = self.tempo_increase = self.accelerando = self.transpose = self.volume = 0


class Channel:
    """A music channel's record, as music_track_start leaves it (cleared) at 'start'"""
    FIELDS = ("octave", "transpose", "loop_transpose", "loop_volume_offset", "volume_offset", "mode", "instrument",
              "volume", "gate", "detune", "portamento", "lfo_ramp", "attenuation", "loop_count", "loop_start",
              "loop_2_count", "loop_2_start", "repeat_count", "repeat_return", "repeat_part", "call_return",
              "call_2_return", "loop_exit_count", "transposed_loop_count", "in_call", "in_call_2")

    def __init__(self, number, start):
        self.number = number                # 0-8: FM 1-7, PCM A, PCM B
        self.pcm = number >= 7
        self.place = start
        for name in self.FIELDS:
            setattr(self, name, 0)

    def state(self):
        """What the channel's future depends on, but its time: a loop's start, a part's return only while they
        are in use"""
        return (self.place, self.octave, self.transpose, self.loop_transpose, self.loop_volume_offset,
                self.volume_offset, self.mode, self.instrument, self.volume, self.gate, self.detune, self.portamento,
                self.lfo_ramp, self.attenuation, self.loop_count, self.loop_2_count, self.loop_exit_count,
                self.transposed_loop_count, self.repeat_count,
                self.loop_start if self.loop_count else None, self.loop_2_start if self.loop_2_count else None,
                (self.repeat_return, self.repeat_part) if self.repeat_count else None,
                self.call_return if self.in_call else None, self.call_2_return if self.in_call_2 else None)

    def execute(self, music, at, data, target):
        """The command 'data' (its bytes) at 'at', 'target' the address it carries: what it does to the record and
        to the music's variables; where the track goes on (None: it ends)"""
        command = data[0]
        if command < 0x10:
            self.octave = command & 0x0F if self.pcm else (24 * (command & 0x0F)) & 0xFF
            return at + 1
        if command & 0xF0 == 0x70:
            self.attenuation = command & 0x0F
            return at + 1
        argument = data[1] if len(data) > 1 else 0
        if command == 0xE0:
            self.mode = argument
        elif command == 0xE1:
            music.tempo = argument
        elif command == 0xE2:
            self.instrument = argument
        elif command == 0xE3:
            self.volume = (argument + (0 if self.pcm else 10)) & 0xFF
            self.attenuation = 0
        elif command == 0xE4:
            self.gate = argument
        elif command == 0xE6:
            self.detune = argument
        elif command == 0xE7:
            self.portamento = argument
        elif command == 0xE8:
            music.transpose = transposition(argument)
        elif command == 0xE9:
            self.transpose = transposition(argument)
        elif command == 0xEA:
            music.volume = attenuation_offset(argument)
        elif command == 0xEB:
            self.volume_offset = attenuation_offset(argument)
        elif command == 0xEC:
            count = self.transposed_loop_count
            if count == 1:
                self.transposed_loop_count = 0
                self.loop_transpose = self.loop_volume_offset = 0
                return at + 6
            if count == 0:
                count = argument
                self.loop_transpose = self.loop_volume_offset = 0
            self.transposed_loop_count = (count - 1) & 0xFF
            self.loop_transpose = (self.loop_transpose + transposition(data[2])) & 0xFF
            self.loop_volume_offset = (self.loop_volume_offset + attenuation_offset(data[3])) & 0xFF
            return target
        elif command == 0xEE:
            self.lfo_ramp = argument
        elif command == 0xF0:
            return target
        elif command == 0xF1:
            count = self.loop_exit_count
            if count == 1:
                self.loop_exit_count = 0
                return target
            self.loop_exit_count = ((argument if count == 0 else count) - 1) & 0xFF
            return at + 4
        elif command in (0xF2, 0xF4):
            second = "_2" if command == 0xF4 else ""
            setattr(self, f"loop{second}_count", argument)
            setattr(self, f"loop{second}_start", at + 2)
            return at + 2
        elif command in (0xF3, 0xF5):
            second = "_2" if command == 0xF5 else ""
            count = getattr(self, f"loop{second}_count")
            setattr(self, f"loop{second}_count", (count - 1) & 0xFF)
            return at + 1 if count == 1 else getattr(self, f"loop{second}_start")
        elif command == 0xF6:
            self.repeat_count = argument
            self.repeat_return = at + 4
            self.repeat_part = target
            return target
        elif command == 0xF7:
            count = self.repeat_count
            self.repeat_count = (count - 1) & 0xFF
            return self.repeat_return if count == 1 else self.repeat_part
        elif command == 0xF8:
            self.call_return, self.in_call = at + 3, 1
            return target
        elif command == 0xF9:
            self.in_call = 0
            return self.call_return
        elif command == 0xFA:
            self.call_2_return, self.in_call_2 = at + 3, 1
            return target
        elif command == 0xFB:
            self.in_call_2 = 0
            return self.call_2_return
        elif command in (0xFC, 0xFD, 0xFE):
            music.accelerando = argument | ((argument & 0x0F) << 4) << 8
            music.tempo_increase = 0
        elif command == 0xFF:
            return None
        return at + len(data)

    # -- the keys of the files' notes --

    def drums(self):
        return self.pcm and self.mode & 1

    def key(self, music, nibble):
        """The key of the note whose pitch nibble is 'nibble' (None: no key has it)"""
        if self.drums():
            return DRUM_KEYS[nibble] if 0 < nibble < len(DRUM_KEYS) else None
        if self.pcm:
            key = ((nibble + 12 * ((self.octave + 1) & 0xFF or 256)) & 0xFF) - 1
        else:
            key = ((music.transpose + 2 * nibble + self.octave + self.transpose + self.loop_transpose) & 0xFF) // 2 + 11
        return key if 0 <= key <= 127 else None

    def nibble(self, music, key):
        """The pitch nibble that gives 'key' (None: none does)"""
        if self.drums():
            return DRUM_KEYS.index(key) if key in DRUM_KEYS[1:] else None
        if self.pcm:
            nibble = (key + 1 - 12 * ((self.octave + 1) & 0xFF or 256)) & 0xFF
        else:
            if key < 11:
                return None
            nibble = ((2 * (key - 11) - music.transpose - self.octave - self.transpose - self.loop_transpose) & 0xFF) // 2
        return nibble if nibble <= 15 and self.key(music, nibble) == key else None

    def channel(self):
        """The MIDI channel of its notes"""
        if self.drums():
            return DRUM_CHANNEL
        return (8 if self.number == 7 else 10) if self.pcm else self.number + 1

    def released(self, length):
        """When the driver keys a note of 'length' ticks off, ticks after it starts: an FM note by its gate"""
        if self.pcm or not self.gate:
            return length
        return max(1, length - (((length & 0xFF) * self.gate >> 8) + 1))


def code_of(length, lengths):
    """The length's code of a note or a rest (1-D but 7, the attenuation step), None if none has it"""
    for code in range(1, min(len(lengths), 13) + 1):
        if code != 7 and lengths[code - 1] == length:
            return code
    return None


# ---- the songs, played pass by pass ----

class Player:
    """A song's 9 tracks from 'starts', played as the driver does, a pass at a time: each channel's reads (the
    commands up to a step: [tick, [(at, bytes, target)], step, ticks]; a step ('note', at, byte), ('rest', at,
    byte), ('held', at, bytes), ('end', at) or None), and the music's tempo by tick"""

    def __init__(self, image, starts, lengths):
        self.image = image
        self.lengths = lengths
        self.music = Music()
        self.channels = [Channel(number, start) for number, start in enumerate(starts)]
        for channel in self.channels:
            channel.reads, channel.seen = [], {}
            channel.handler, channel.tick, channel.tempo_count, channel.ticks_left = "start", 0, 0, 0
            channel.done = None          # the tick it comes back to a state it had, or ends
        self.tempos = []
        self.passes = 0
        self.last_tempo = None
        self.clock = 3 * 1024 + 300     # the Z80's cycles when this pass started timer B
        self.clocks = {0: self.clock}   # tick -> the Z80's cycles at the pass that first reached it
        self.work = 0

    def u16(self, at):
        return self.image[at] | self.image[at + 1] << 8

    def command_bytes(self, at):
        command = self.image[at]
        if command < 0xE0:
            return bytes([command]), None
        length = COMMAND_LENGTHS[command]
        target = self.u16(at + ADDRESS_AT[command]) if command in ADDRESS_AT else None
        return bytes(self.image[at:at + length]), target

    def read(self, channel):
        """read_track, and its work in the pass"""
        self.read_track(channel)
        tick, items, step, ticks = channel.reads[-1]
        kind = "note" if step and step[0] in ("note", "held") else "rest"
        self.work += PASS_WORK[("pcm_" if channel.pcm else "fm_") + kind] + PASS_WORK["command"] * len(items)
        if not channel.pcm:
            self.work += PASS_WORK["instrument"] * sum(1 for at, data, target in items if data[0] == 0xE2)

    def read_track(self, channel):
        """The channel's commands up to its next step, as a read (and whether its state is one it had)"""
        state = channel.state()
        if channel.done is None and state in channel.seen:
            channel.done = channel.tick
        channel.seen.setdefault(state, channel.tick)
        read = [channel.tick, [], None, 0]
        channel.reads.append(read)
        while True:
            at = channel.place
            byte = self.image[at]
            if byte & 0xF0 in (0x00, 0x70) or (byte >= 0xE0 and byte != 0xE5):
                data, target = self.command_bytes(at)
                read[1].append((at, data, target))
                place = channel.execute(self.music, at, data, target)
                if place is None:
                    read[2] = ("end", at)
                    channel.handler = None
                    if channel.done is None:
                        channel.done = channel.tick
                    return
                channel.place = place
                continue
            if byte == 0xE5:
                data = bytes(self.image[at + 1:at + 1 + held_length(self.image[at + 1:at + 40])])
                nibble = data[-1] & 0x0F if channel.pcm and not channel.drums() else data[0] & 0x0F
                read[2] = ("held", at, bytes([0xE5]) + data, channel.key(self.music, nibble))
                channel.ticks_left = held_ticks(data, self.lengths) & 0xFF
                read[3] = held_ticks(data, self.lengths)
                channel.place = at + 1 + len(data)
            else:
                read[2] = ("rest" if not byte & 0x0F else "note", at, bytes([byte]),
                           channel.key(self.music, byte & 0x0F))
                channel.ticks_left = self.lengths[(byte >> 4) - 1]
                read[3] = channel.ticks_left
                channel.place = at + 1
            return

    def tick_now(self):
        ticks = [channel.tick for channel in self.channels if channel.handler is not None]
        return max(ticks) if ticks else max(channel.tick for channel in self.channels)

    def note_tempo(self):
        tempo = (self.music.tempo + self.music.tempo_increase) & 0xFF
        if tempo != self.last_tempo:
            tick = self.tick_now()
            if self.tempos and self.tempos[-1][0] == tick:
                self.tempos.pop()
            self.tempos.append((tick, tempo))
            self.last_tempo = tempo

    def pass_time(self):
        """The pass's end and the next one's start: timer B runs out 16 x 14 samples after its start, short by the
        low 4 bits of the sample count (ymfm: its divide by 16 runs free), which the main loop finds after the
        pass's work, at a turn of its wait; then it starts timer B again"""
        samples = -(-self.clock // 64)
        expiry = self.clock + (TIMER_B_SAMPLES - (samples & 15)) * 64
        end = self.clock + PASS_WORK["base"] + self.work
        found = end if end >= expiry else end + POLL_CYCLES * -(-(expiry - end) // POLL_CYCLES)
        self.clock = found + RESTART_CYCLES
        self.work = 0
        self.clocks.setdefault(self.tick_now(), self.clock)

    def tempo_events(self, end):
        """[(tick, microseconds a quarter note)] where the tempo changes: each stretch's ticks at the time its passes
        take (pass_time)"""
        ticks = sorted(self.clocks)
        clock_at = {}
        for tick in range(ticks[-1] + 1):
            clock_at[tick] = self.clocks[tick] if tick in self.clocks else clock_at[tick - 1]
        out = []
        changes = [(tick, tempo) for tick, tempo in self.tempos if tick <= end]
        for index, (tick, tempo) in enumerate(changes):
            stop = changes[index + 1][0] if index + 1 < len(changes) else min(end, ticks[-1])
            if not tempo:
                continue
            if stop > tick and stop in clock_at:
                cycles = (clock_at[stop] - clock_at[tick]) / (stop - tick)
            else:
                cycles = 256 / tempo * TIMER_B_SAMPLES * 64
            out.append((tick, round(cycles * TICKS_A_QUARTER * 1_000_000 / SOUND_CLOCK)))
        return out

    def step(self):
        """A pass of the main loop: the music's channels"""
        self.passes += 1
        music = self.music
        for channel in self.channels:
            if channel.handler == "start":
                channel.handler = "first"
            elif channel.handler == "first":
                channel.handler = "tick"
                self.read(channel)
            elif channel.handler == "tick":
                if channel.number == 0 and music.accelerando & 0x0F:
                    count = ((music.accelerando >> 8) - 1) & 0xFF
                    if count == 0:
                        count = (music.accelerando & 0x0F) << 4
                        music.tempo_increase = (music.tempo_increase + 1) & 0xFF
                    music.accelerando = (music.accelerando & 0xFF) | count << 8
                tempo = (music.tempo_increase + music.tempo) & 0xFF
                if tempo == 0:
                    channel.handler = None
                    if channel.done is None:
                        channel.done = channel.tick
                    continue
                channel.tempo_count += tempo
                if channel.tempo_count > 0xFF:
                    channel.tempo_count &= 0xFF
                    channel.tick += 1
                    left = channel.ticks_left
                    channel.ticks_left = (left - 1) & 0xFF
                    if left == 1:
                        self.read(channel)
            self.note_tempo()
        self.pass_time()

    def run(self):
        """Plays until every channel has come back to a state it had or ended (or CAP_PASSES passes), and then
        until each has read past that tick: the tick where the file stops showing the song"""
        end = None
        while True:
            self.step()
            if end is None and (all(channel.done is not None for channel in self.channels)
                                or self.passes >= CAP_PASSES):
                end = max(channel.done if channel.done is not None else channel.tick for channel in self.channels)
            if end is not None and all(channel.handler is None or channel.reads[-1][0] >= end
                                       for channel in self.channels):
                return end

    def shown(self, end):
        """Each channel's reads the file shows: those before 'end', then the first one at or after it, its
        commands only (its step a cut), unless it ends the track; the last read of a channel the tempo stopped,
        and then a cut"""
        tracks = []
        for channel in self.channels:
            reads = []
            for tick, items, step, ticks in channel.reads:
                if tick < end:
                    reads.append((tick, items, step, ticks))
                    continue
                if step is not None and step[0] == "end":
                    reads.append((tick, items, step, ticks))
                else:
                    reads.append((tick, items, ("cut",), 0))
                break
            else:
                if reads and reads[-1][2][0] != "end":
                    last = reads[-1]
                    reads.append((last[0] + last[3], [], ("cut",), 0))
            tracks.append(reads)
        return tracks


# ---- the files ----

def vlq(value):
    """A variable-length quantity"""
    out = [value & 0x7F]
    value >>= 7
    while value:
        out.append(0x80 | (value & 0x7F))
        value >>= 7
    return bytes(reversed(out))


def meta(kind, data):
    return bytes([0xFF, kind]) + vlq(len(data)) + data


def driver_event(tag, data=b""):
    return meta(0x7F, bytes([TAG, ord(tag)]) + data)


class Track:
    """A track's events, in order at a tick, Note Offs first"""

    def __init__(self):
        self.events = []

    def add(self, tick, data, note_off=False):
        self.events.append((tick, 0 if note_off else 1, len(self.events), data))

    def chunk(self):
        out = bytearray()
        now = 0
        for tick, _, _, data in sorted(self.events):
            out += vlq(tick - now) + data
            now = tick
        out += b"\x00\xFF\x2F\x00"
        return b"MTrk" + len(out).to_bytes(4, "big") + bytes(out)


class Names:
    """The places' names: 'pieces' {first: (stop, name)}"""

    def __init__(self, pieces):
        self.starts = sorted(pieces)
        self.pieces = pieces
        self.addresses = {name: first for first, (_, name) in pieces.items()}

    def name(self, address):
        index = bisect_right(self.starts, address) - 1
        if index >= 0:
            first = self.starts[index]
            stop, name = self.pieces[first]
            if first <= address < stop:
                return name if address == first else f"{name}+0x{address - first:X}"
        raise MusicError(f"{address:04X} is in no piece of the sound program")

    def address(self, text, where):
        base, plus, offset = text.partition("+")
        if base not in self.addresses or (plus and not re.fullmatch(r"0x[0-9A-Fa-f]+", offset)):
            raise MusicError(f"{where}: no place is named {text}")
        return self.addresses[base] + (int(offset, 16) if plus else 0)


def replay_keys(tracks, levels):
    """The notes of the shown reads ('tracks', Player.shown): {(track, read): (key, velocity, MIDI channel, ticks to
    its Note Off)}, and each FM instrument's keys. The reads go in the order the way back reads them (by tick, then
    by track), the driver's record kept as the way back keeps it"""
    music = Music()
    channels = [Channel(number, 0) for number in range(len(tracks))]
    order = sorted((read[0], number, index) for number, reads in enumerate(tracks) for index, read in enumerate(reads))
    notes = {}
    played = defaultdict(list)
    for _, number, index in order:
        channel = channels[number]
        tick, items, step, ticks = tracks[number][index]
        for at, data, target in items:
            channel.execute(music, at, data, target)
        if step[0] not in ("note", "held"):
            continue
        data = step[2]
        if step[0] == "note":
            nibble = data[0] & 0x0F
        elif channel.pcm and not channel.drums():
            nibble = data[-1] & 0x0F
        else:
            nibble = data[1] & 0x0F
        key = channel.key(music, nibble)
        if key is None or channel.nibble(music, key) != nibble:
            raise MusicError(f"{CHANNEL_NAMES[number]}, tick {tick}: the pitch {nibble} has no key")
        if key != step[3]:
            raise MusicError(f"{CHANNEL_NAMES[number]}, tick {tick}: the note plays key {step[3]}, and the way back "
                             f"reads it as {key} (the music's transposition by tick and track)")
        if channel.pcm:
            level = levels["drums"][nibble - 1] if channel.drums() else levels["instruments"][channel.instrument & 3]
            total = (channel.volume + channel.attenuation + channel.loop_volume_offset) & 0xFF
            velocity = round(127 * math.sqrt(max(0, level - total) / 15))
        else:
            total = (music.volume + channel.volume_offset + channel.loop_volume_offset + channel.attenuation
                     + channel.volume) & 0xFF
            velocity = round(127 * 10 ** (-0.75 * (0x7F if total & 0x80 else total) / 40))
            played[channel.instrument].append(key)
        notes[(number, index)] = (key, max(1, min(127, velocity)), channel.channel(), channel.released(ticks))
    return notes, played


def song_file(name, numbers, starts, tracks, tempos, end, notes, programs, names, unshown):
    """A song's Standard MIDI File"""
    songs = ", ".join(f"{number} (command {FIRST_SONG + number:02X})" for number in numbers)
    first = Track()
    first.add(0, meta(0x03, f"TMNT {name}: the songs table's {songs}".encode("ascii")))
    first.add(0, driver_event("N", bytes(numbers)))
    for tick, quarter in tempos:
        first.add(tick, meta(0x51, min(quarter, 0xFFFFFF).to_bytes(3, "big")))
    for place, data in unshown:
        first.add(0, driver_event("U", place.encode("ascii") + b"\x00" + data))
    chunks = [first.chunk()]
    for number, reads in enumerate(tracks):
        track = Track()
        track.add(0, meta(0x03, CHANNEL_NAMES[number].encode("ascii")))
        track.add(0, driver_event("S", names.name(starts[number]).encode("ascii")))
        track.add(0, bytes([0xC0 | (number + 1), programs.get(0, FM_LEAD)]) if number < 7 else
                  bytes([0xC8 if number == 7 else 0xCA, PCM_BASS]))
        for index, (tick, items, step, ticks) in enumerate(reads):
            for at, data, target in items:
                command = data[0]
                if command in ADDRESS_AT:
                    data = data[:ADDRESS_AT[command]] + names.name(target).encode("ascii")
                track.add(tick, driver_event("C", data))
                if command == 0xE2 and number < 7:
                    track.add(tick, bytes([0xC0 | (number + 1), programs.get(data[1], FM_LEAD)]))
            kind = step[0]
            if kind == "rest":
                track.add(tick, driver_event("R"))
            elif kind == "cut":
                track.add(tick, driver_event("Z"))
            elif kind in ("note", "held"):
                if kind == "held":
                    track.add(tick, driver_event("H", step[2][1:]))
                key, velocity, channel, released = notes[(number, index)]
                track.add(tick, bytes([0x90 | channel, key, velocity]))
                track.add(tick + released, bytes([0x80 | channel, key, 0]), note_off=True)
        chunks.append(track.chunk())
    header = b"MThd" + (6).to_bytes(4, "big") + (1).to_bytes(2, "big") + len(chunks).to_bytes(2, "big") \
        + TICKS_A_QUARTER.to_bytes(2, "big")
    return header + b"".join(chunks)


def song_owner(name, owners):
    """The songs table entry a piece's bytes belong to, by its name (songs_N_..., song_XX_...); None: the one
    before it"""
    match = re.match(r"songs_(\d+)_", name)
    if match:
        return int(match.group(1))
    match = re.match(r"song_([0-9a-f]{2})_", name)
    return int(match.group(1), 16) - FIRST_SONG if match else None


def write(image, pieces, folder):
    """The songs of the sound program 'image' as MIDI files into 'folder', and music.txt; 'pieces'
    [(name, kind, first, stop)] are sound_data_assets' (the music's kind 'music', the table 'songs'). Returns the
    files' count and the names of the pieces they give back"""
    by_name = {name: (first, stop) for name, _, first, stop in pieces}
    names = Names({first: (stop, name) for name, _, first, stop in pieces})
    lengths = list(image[by_name["note_lengths"][0]:by_name["note_lengths"][1]])
    drums, instruments = by_name["pcm_drums"][0], by_name["pcm_instruments"][0]
    levels = {"drums": [image[drums + 6 * index + 3] for index in range(15)],
              "instruments": [image[instruments + 4 * index + 3] for index in range(4)]}
    table, table_stop = by_name["songs"]
    count = (table_stop - table) // (2 * SONG_TRACKS)

    # a file for each different set of tracks, named by the first of its songs the game names
    files = {}
    for number in range(count):
        starts = tuple(image[table + 2 * (SONG_TRACKS * number + track)]
                       | image[table + 2 * (SONG_TRACKS * number + track) + 1] << 8 for track in range(SONG_TRACKS))
        files.setdefault(starts, []).append(number)
    songs = []
    for starts, numbers in files.items():
        named = [FIRST_SONG + number for number in numbers if FIRST_SONG + number in SONGS]
        name = SONGS[named[0]][0] if named else f"song_{FIRST_SONG + numbers[0]:02x}"
        player = Player(image, starts, lengths)
        end = player.run()
        tracks = player.shown(end)
        notes, played = replay_keys(tracks, levels)
        songs.append({"name": name, "numbers": numbers, "starts": starts, "tracks": tracks, "end": end,
                      "tempos": player.tempo_events(end), "notes": notes, "played": played})

    # the FM instruments' programs: a bass below key 48
    keys = defaultdict(list)
    for song in songs:
        for instrument, played in song["played"].items():
            keys[instrument] += played
    programs = {instrument: FM_BASS if sorted(played)[len(played) // 2] < 48 else FM_LEAD
                for instrument, played in keys.items()}

    # the bytes no file shows, in the file of the song their piece belongs to
    shown = set()
    for song in songs:
        for reads in song["tracks"]:
            for tick, items, step, ticks in reads:
                for at, data, target in items:
                    shown.update(range(at, at + len(data)))
                if step[0] in ("note", "rest", "held"):
                    shown.update(range(step[1], step[1] + len(step[2])))
    file_of = {number: song for song in songs for number in song["numbers"]}
    owner = None
    unshown = defaultdict(list)
    music = [(first, stop, name) for name, kind, first, stop in pieces if kind.startswith("music")]
    for first, stop, name in sorted(music):
        owner = song_owner(name, file_of) if song_owner(name, file_of) is not None else owner
        run = None
        for address in list(range(first, stop)) + [None]:
            if address is not None and address not in shown:
                run = run or [address, address]
                run[1] = address + 1
                continue
            if run:
                unshown[file_of[owner]["name"]].append((names.name(run[0]), bytes(image[run[0]:run[1]])))
                run = None

    folder.mkdir(parents=True, exist_ok=True)
    for old in folder.glob("*.mid"):
        if old.stem not in {song["name"] for song in songs}:
            old.unlink()
    listing = ["# the songs (tools/sound_music.py): each MIDI file, the songs table's entries it plays (their commands",
               "# 60 on), and what the game plays it for"]
    for song in songs:
        data = song_file(song["name"], song["numbers"], song["starts"], song["tracks"], song["tempos"], song["end"],
                         song["notes"], programs, names, unshown.get(song["name"], []))
        (folder / f"{song['name']}.mid").write_bytes(data)
        uses = "; ".join(SONGS[FIRST_SONG + number][1] for number in song["numbers"] if FIRST_SONG + number in SONGS)
        listing.append(f"{song['name']}  {' '.join(str(number) for number in song['numbers'])}  # "
                       + ", ".join(f"{FIRST_SONG + number:02X}" for number in song["numbers"])
                       + (f": {uses}" if uses else ""))
    (folder / "music.txt").write_text("\n".join(listing) + "\n", encoding="ascii")
    return len(songs), [name for _, _, name in music] + ["songs"]


# ---- the way back ----

def parse(data, where):
    """A Standard MIDI File: (division, [track's events: (tick, kind, values)]), kinds 'on' (channel, key, velocity),
    'off' (channel, key) and 'meta' (type, bytes); the others left out"""
    if data[:4] != b"MThd" or len(data) < 14:
        raise MusicError(f"{where} is no Standard MIDI File")
    division = int.from_bytes(data[12:14], "big")
    if division & 0x8000 or not division:
        raise MusicError(f"{where}: its time is no ticks a quarter note")
    at = 8 + int.from_bytes(data[4:8], "big")
    tracks = []
    while at + 8 <= len(data):
        kind, size = data[at:at + 4], int.from_bytes(data[at + 4:at + 8], "big")
        body = data[at + 8:at + 8 + size]
        at += 8 + size
        if kind != b"MTrk":
            continue
        events, tick, place, status = [], 0, 0, None

        def number():
            nonlocal place
            value = 0
            for _ in range(4):
                if place >= len(body):
                    raise MusicError(f"{where}: track {len(tracks) + 1} ends inside an event")
                byte = body[place]
                place += 1
                value = value << 7 | byte & 0x7F
                if not byte & 0x80:
                    return value
            raise MusicError(f"{where}: track {len(tracks) + 1}: a variable-length quantity of more than 4 bytes")

        while place < len(body):
            tick += number()
            if place >= len(body):
                raise MusicError(f"{where}: track {len(tracks) + 1} ends inside an event")
            byte = body[place]
            if byte == 0xFF:
                if place + 1 >= len(body):
                    raise MusicError(f"{where}: track {len(tracks) + 1} ends inside an event")
                kind_ = body[place + 1]
                place += 2
                size_ = number()
                if kind_ == 0x2F:
                    break
                events.append((tick, "meta", (kind_, bytes(body[place:place + size_]))))
                place += size_
                continue
            if byte in (0xF0, 0xF7):
                place += 1
                place += number()
                continue
            if byte & 0x80:
                if byte > 0xF0:
                    raise MusicError(f"{where}: track {len(tracks) + 1}, tick {tick}: a status byte {byte:02X}")
                status = byte
                place += 1
            elif status is None:
                raise MusicError(f"{where}: track {len(tracks) + 1}, tick {tick}: data with no status before it")
            size_ = 1 if status & 0xE0 == 0xC0 else 2
            if place + size_ > len(body):
                raise MusicError(f"{where}: track {len(tracks) + 1} ends inside an event")
            values = body[place:place + size_]
            place += size_
            if status & 0xF0 == 0x90 and values[1]:
                events.append((tick, "on", (status & 0x0F, values[0], values[1])))
            elif status & 0xF0 in (0x80, 0x90):
                events.append((tick, "off", (status & 0x0F, values[0])))
        tracks.append(events)
    return division, tracks


class Rebuilt:
    """The bytes the files give, by address, and where each came from"""

    def __init__(self, names):
        self.names = names
        self.bytes = {}

    def put(self, address, data, where):
        for offset, byte in enumerate(data):
            place = address + offset
            if place in self.bytes and self.bytes[place][0] != byte:
                raise MusicError(f"{where}: the byte at {self.names.name(place)} comes back as {byte:02X}, and as "
                                 f"{self.bytes[place][0]:02X} from {self.bytes[place][1]}")
            self.bytes.setdefault(place, (byte, where))


def driver_tracks(tracks, where):
    """The tracks with an 'S' event, as [(start name, [read: [tick, [command bytes], step, Note Off tick]])], and the
    file's 'N' and 'U' events. A read is a tick's events, the commands and then a step: ('note', key), ('rest',),
    ('held', bytes, key), ('cut',); after a command FF, nothing"""
    numbers, unshown, found = None, [], []
    for index, events in enumerate(tracks):
        start, reads, open_note, ended = None, [], None, False
        here = f"{where}, track {index + 1}"
        for tick, kind, values in sorted(events, key=lambda event: (event[0], event[1] != "off")):
            if kind == "off":
                if open_note is not None and open_note[0] == values[1]:
                    open_note[1][3] = tick
                    open_note = None
                continue
            if kind == "meta":
                if values[0] != 0x7F or values[1][:1] != bytes([TAG]) or len(values[1]) < 2:
                    continue
                tag, data = chr(values[1][1]), values[1][2:]
                if tag == "N":
                    numbers = list(data)
                    continue
                if tag == "U":
                    place, zero, rest = data.partition(b"\x00")
                    if not zero:
                        raise MusicError(f"{here}, tick {tick}: a 'U' event with no place")
                    unshown.append((place.decode("ascii", "replace"), rest, f"{here}, tick {tick}"))
                    continue
                if tag == "S":
                    start = data.decode("ascii", "replace")
                    continue
                if tag not in "CRHZ":
                    continue
            if start is None:
                raise MusicError(f"{here}, tick {tick}: the driver's events come before its start ('S')")
            if ended:
                raise MusicError(f"{here}, tick {tick}: an event after the channel's end")
            if not reads or reads[-1][0] != tick:
                if reads and reads[-1][2] is None:
                    raise MusicError(f"{here}, tick {reads[-1][0]}: commands with no step after them")
                reads.append([tick, [], None, None])
            read = reads[-1]
            if kind == "on":
                if read[2] is not None and not (read[2][0] == "held" and read[2][2] is None):
                    raise MusicError(f"{here}, tick {tick}: a second step at a tick")
                if open_note is not None:
                    raise MusicError(f"{here}, tick {tick}: a note starts before the one at tick {open_note[1][0]} "
                                     f"ends")
                read[2] = ("held", read[2][1], values[1]) if read[2] is not None else ("note", values[1])
                open_note = (values[1], read)
                continue
            if read[2] is not None:
                raise MusicError(f"{here}, tick {tick}: an event after the tick's step")
            if tag == "C":
                if not data:
                    raise MusicError(f"{here}, tick {tick}: a command with no bytes")
                read[1].append(data)
                ended = data[0] == 0xFF
            elif tag == "R":
                read[2] = ("rest",)
            elif tag == "H":
                read[2] = ("held", data, None)
            else:
                read[2] = ("cut",)
                ended = True
        if start is None:
            continue
        if reads and reads[-1][2] is None and not ended:
            raise MusicError(f"{here}, tick {reads[-1][0]}: commands with no step after them")
        found.append((start, reads, here))
    return numbers, unshown, found


def rebuild(files, names, lengths, songs_table, out):
    """The way back: the bytes the MIDI files 'files' ([(name, bytes)]) give into 'out' (Rebuilt); 'names' the
    layout's (Names), 'lengths' note_lengths, 'songs_table' the songs table's address"""
    for file_name, data in files:
        division, tracks = parse(data, file_name)
        numbers, unshown, found = driver_tracks(tracks, file_name)
        if numbers is None:
            raise MusicError(f"{file_name}: no 'N' event says its songs")
        if len(found) != SONG_TRACKS:
            raise MusicError(f"{file_name}: {len(found)} tracks with a start ('S'); a song has {SONG_TRACKS}")
        # the driver's ticks
        for _, reads, here in found:
            for read in reads:
                for slot in (0, 3):
                    if read[slot] is not None:
                        if read[slot] * TICKS_A_QUARTER % division:
                            raise MusicError(f"{here}: the tick {read[slot]} is no whole driver's tick")
                        read[slot] = read[slot] * TICKS_A_QUARTER // division
        music = Music()
        channels = []
        for number, (start, reads, here) in enumerate(found):
            channel = Channel(number, names.address(start, f"{here}, its start"))
            channel.reads, channel.here, channel.done = reads, here, False
            channels.append(channel)
        for number in numbers:
            for track, channel in enumerate(channels):
                rebuilt_start = names.address(found[track][0], found[track][2])
                out.put(songs_table + 2 * (SONG_TRACKS * number + track), rebuilt_start.to_bytes(2, "little"),
                        f"{file_name}'s 'N'")
        order = sorted((read[0], number, index) for number, channel in enumerate(channels)
                       for index, read in enumerate(channel.reads))
        for tick, number, index in order:
            channel = channels[number]
            here = f"{channel.here}, tick {tick}"
            if channel.done:
                raise MusicError(f"{here}: the channel goes on after its end")
            _, commands, step, released = channel.reads[index]
            following = channel.reads[index + 1][0] if index + 1 < len(channel.reads) else None
            for data in commands:
                command = data[0]
                if 0x10 <= command < 0x70 or 0x80 <= command < 0xE0 or command == 0xE5:
                    raise MusicError(f"{here}: {command:02X} is no command ('C')")
                target = None
                if command in ADDRESS_AT:
                    text = data[ADDRESS_AT[command]:].decode("ascii", "replace")
                    target = names.address(text, here)
                    data = data[:ADDRESS_AT[command]] + target.to_bytes(2, "little")
                if len(data) != COMMAND_LENGTHS.get(command, 1):
                    raise MusicError(f"{here}: the command {command:02X} has {len(data)} bytes; the driver reads "
                                     f"{COMMAND_LENGTHS.get(command, 1)}")
                out.put(channel.place, data, here)
                place = channel.execute(music, channel.place, data, target)
                if place is None:
                    channel.done = True
                    break
                channel.place = place
            if channel.done or step is None:
                continue
            if step[0] == "cut":
                channel.done = True
                continue
            if following is None:
                raise MusicError(f"{here}: no step after it says how long it is")
            length = following - tick
            if step[0] == "held":
                held = step[1]
                if not held or held_length(held) != len(held):
                    raise MusicError(f"{here}: the held note's bytes ('H') do not end at their first 0 nibble")
                if held_ticks(held, lengths) is None:
                    raise MusicError(f"{here}: the held note's bytes ('H') have a length's code note_lengths has not")
                if held_ticks(held, lengths) != length:
                    raise MusicError(f"{here}: the held note lasts {length} ticks; its lengths ('H') add up to "
                                     f"{held_ticks(held, lengths)}")
                if step[2] is None:
                    raise MusicError(f"{here}: the held note ('H') has no Note On")
                if channel.pcm and not channel.drums():
                    if channel.key(music, held[-1] & 0x0F) != step[2]:
                        raise MusicError(f"{here}: the held note plays key {step[2]}; its bytes ('H') give "
                                         f"{channel.key(music, held[-1] & 0x0F)}")
                else:
                    nibble = channel.nibble(music, step[2])
                    if nibble is None:
                        raise MusicError(f"{here}: no pitch gives the key {step[2]} from the channel's octave and "
                                         f"transpositions")
                    held = bytes([held[0] & 0xF0 | nibble]) + held[1:]
                data = bytes([0xE5]) + held
            else:
                code = code_of(length, lengths)
                if code is None:
                    raise MusicError(f"{here}: no note or rest lasts {length} ticks (note_lengths)")
                nibble = 0
                if step[0] == "note":
                    nibble = channel.nibble(music, step[1])
                    if not nibble:
                        raise MusicError(f"{here}: no pitch gives the key {step[1]} from the channel's octave and "
                                         f"transpositions")
                data = bytes([code << 4 | nibble])
            if step[0] != "rest":
                expected = tick + channel.released(length)
                if released is None:
                    raise MusicError(f"{here}: the note has no Note Off")
                if released != expected:
                    raise MusicError(f"{here}: the note ends at tick {released}; the driver keys it off at "
                                     f"{expected}")
            out.put(channel.place, data, here)
            channel.place += len(data)
        for place, data, here in unshown:
            out.put(names.address(place, here), data, here)
    return out


def read(folder, names, lengths, songs_table):
    """The music the MIDI files of music.txt in 'folder' give: a Rebuilt"""
    listing = folder / "music.txt"
    if not listing.exists():
        raise MusicError(f"no {listing.name} in {folder}")
    files = []
    for line in listing.read_text(encoding="ascii").splitlines():
        if line.strip() and not line.startswith("#"):
            name = line.split()[0]
            path = folder / f"{name}.mid"
            if not path.exists():
                raise MusicError(f"music.txt names {name}, and there is no {path.name}")
            files.append((path.name, path.read_bytes()))
    return rebuild(files, names, lengths, songs_table, Rebuilt(names))
