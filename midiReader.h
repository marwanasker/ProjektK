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

class MidiReader{
public:
MidiReader(string fileName, int track, int argc, char** argv);
};
