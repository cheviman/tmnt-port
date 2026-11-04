"""
Name:        sound_data_walk.py
Description: the music the sound driver plays, walked from where its song table points as the driver reads it
Version:     0.1.0
Created:     29-10-2026
Modified:    29-10-2026
Author:      Sebastian Toyos

The sound coverage (tools/core/sound_coverage.py, the profile's SOUND_DATA_WALK) hands spans() the places
sound_symbols.txt's tables point at whose length only the data says: here the music tracks, the 9 a song
(songs: 7 on the YM2151's channels 1-7, then the K007232's two), which read_track_command (0A46) reads a
command at a time. A byte below E0 is one byte: 0x the octave (track_set_octave), 7x the volume step
(track_set_volume_step), any other a note, its length by its high nibble (note_lengths) and its pitch by its
low one (0: a rest). E0-FF take arguments (the jump tables track_commands_e0, track_commands_f0):
  1 argument   E0-E4, E6-EB, EE, EF, F2, F4, FC-FE
  2            ED (the YM2151's LFO)
  E5           a note held over several lengths: a byte of a length and a pitch, then lengths two a byte up
               to the first nibble that is 0 (that byte too)
  EC           a count, two transpositions and an address: jumps there (the count times), else goes on
  F0           an address: jumps there
  F1           a count and an address: goes on (the count times less one), then jumps there
  F6           a count and an address: plays the part there the count times (F7 returns from it)
  F8, FA       an address: plays the part there (F9, FB return)
  F3, F5       no argument: the ends of the loops F2 and F4 start (they go back to after them)
  F7, F9, FB   no argument: the returns
  FF           no argument: the end of the track
Every place a track jumps or calls to is walked too, as part of the music. The walk is right when every
byte the track reader and its commands read in the scenarios is in what it walked: the coverage's --reads
(the reference's sound_rom_reads.txt, kref --sound-rom-reads) lists the bytes read that nothing describes.
"""
from collections import defaultdict

TRACK = "music_track"          # the elements sound_symbols.txt's song table points at
FIXED = {0xE0: 2, 0xE1: 2, 0xE2: 2, 0xE3: 2, 0xE4: 2, 0xE6: 2, 0xE7: 2, 0xE8: 2, 0xE9: 2, 0xEA: 2, 0xEB: 2,
         0xED: 3, 0xEE: 2, 0xEF: 2, 0xF2: 2, 0xF3: 1, 0xF4: 2, 0xF5: 1, 0xFC: 2, 0xFD: 2, 0xFE: 2}
JUMPS = {0xEC: (6, 4), 0xF1: (4, 2)}        # command: (length, offset of the address), and it goes on
CALLS = {0xF6: (4, 2), 0xF8: (3, 1), 0xFA: (3, 1)}
STOPS = {0xF0, 0xF7, 0xF9, 0xFB, 0xFF}      # no way on: a jump, the returns and the end (one byte, but F0)


class Walk:
    """The tracks from 'starts' ({address: where it comes from}) in the sound program 'image'"""

    def __init__(self, image, starts, code_end):
        self.image = image
        self.code_end = code_end
        self.commands = {}          # address -> (length, track it was first reached from)
        self.problems = []
        self.parts = set()          # the places tracks call
        for start, source in sorted(starts.items()):
            self.track(start, source)
        # a command that starts inside another one: one of the two readings is wrong
        for place, (length, source) in sorted(self.commands.items()):
            for inside in range(place + 1, place + length):
                if inside in self.commands:
                    self.problems.append(f"{self.commands[inside][1]}: a command at {inside:04X}, inside the one "
                                         f"at {place:04X} ({source})")
        self.spans = {}
        for first, stop, source in self.runs():
            self.spans[first] = (stop, f"music: {source}")

    def track(self, start, source):
        work = [(start, source)]
        while work:
            place, source = work.pop()
            while place not in self.commands:
                if not 0 <= place < self.code_end:
                    self.problems.append(f"{source}: a track goes on at {place:04X}, outside the program")
                    break
                command = self.image[place]
                if command < 0xE0:
                    length = 1
                elif command == 0xE5:
                    length = self.held_note(place)
                elif command in FIXED:
                    length = FIXED[command]
                elif command in JUMPS:
                    length, offset = JUMPS[command]
                    work.append((self.address(place + offset), source))
                elif command in CALLS:
                    length, offset = CALLS[command]
                    part = self.address(place + offset)
                    self.parts.add(part)
                    work.append((part, f"{source}'s part {part:04X}"))
                elif command == 0xF0:
                    length = 3
                    work.append((self.address(place + 1), source))
                else:
                    length = 1      # F7, F9, FB, FF
                self.commands[place] = (length, source)
                if command in STOPS:
                    break
                place += length

    def held_note(self, place):
        """E5's length: the command, its first byte, then the bytes up to the first 0 nibble"""
        at = place + 2
        while at < self.code_end:
            byte = self.image[at]
            if byte & 0xF0 == 0 or byte & 0x0F == 0:
                return at + 1 - place
            at += 1
        return at - place

    def address(self, place):
        return self.image[place] | (self.image[place + 1] << 8)

    def runs(self):
        """The bytes walked, as [first, stop, the track or part they were first reached from] runs; a part by its
        number among the parts its track (or part) calls, in the order of their addresses ("songs[3].tracks[0]'s
        part 2"), not by its address"""
        found = []
        for place in sorted(self.commands):
            length, source = self.commands[place]
            if found and place <= found[-1][1] and source == found[-1][2]:
                found[-1][1] = max(found[-1][1], place + length)
            else:
                found.append([place, place + length, source])
        # the parts each caller calls, numbered by their addresses
        parts = defaultdict(set)
        for _, source in self.commands.values():
            caller = source
            while "'s part " in caller:
                caller, _, address = caller.rpartition("'s part ")
                parts[caller].add(int(address, 16))

        def numbered(source):
            if "'s part " not in source:
                return source
            caller, _, address = source.rpartition("'s part ")
            return f"{numbered(caller)}'s part {sorted(parts[caller]).index(int(address, 16)) + 1}"

        return [[first, stop, numbered(source)] for first, stop, source in found]


def spans(program, starts):
    """For the sound coverage (the profile's SOUND_DATA_WALK): {first: (stop, kind)} of what the walk reads
    from 'starts' ({elements name: {address: where it comes from}}), and the problems it met"""
    walk = Walk(program.image, starts.get(TRACK, {}), program.code_end)
    return walk.spans, walk.problems
