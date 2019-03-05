//
//  midiReader.h
//  midiReader
//
//  Created by Marwan Asker on 2019-02-27.
//  Copyright © 2019 Marwan Asker. All rights reserved.
//
#ifndef midiReader_h
#define midiReader_h


#endif /* midiReader_h */
using namespace std;
struct MIDI_command {
    int timestamp;
    int key;
} ;

struct MIDI_note {
    int timestamp; // time
    int key; // note
    double duration;
} ;

class MidiReader{
public:
MidiReader(string fileName, int track_);
void getMidiCommands(MIDI_command commands[]);
void getMidiDuration(MIDI_note notes[]);
int fileLength;
};


