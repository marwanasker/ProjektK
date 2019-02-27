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

struct MIDI_command {
    int timestamp;
    int key;
} ;

int getMidiLength(int argc, char** argv);

void readMidiFile(int argc, char** argv, MIDI_command result[]);
