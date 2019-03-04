#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace smf;

// create a struct that will be loaded with the midi-commands that are essential for the compare function.
//This will include the note that is being played and the tick (the precise time it's played).
struct MIDI_command {
    int timestamp; // time
    int key; // note
} ;

// the MidiReader class. This class is the foundation behind reading a midi file. it will take in a fileName string and a int track.
//the filename is a string, and the track is what part of the midiFile that should be played.
//Since we will be creating our own midifile later we can set int track_ = 0. the fileLenght gives the length of the midifile in amount of ticks.

class MidiReader{
public:
    int fileLength = 0;
    MidiReader(string fileName, int track_);
    void getMidiCommands(MIDI_command commands[]); //this function returns the note and tick(midi_commands) of the midifile.

private:
    MidiFile midifile;
    int track;
};

MidiReader:: MidiReader(string fileName, int track_){ //loaded constructor
    midifile.read(fileName); // read a file
    midifile.doTimeAnalysis(); // analyse the ticks and time of the midifile
    midifile.linkNotePairs(); // analyse the notes being played
    track = track_;
    fileLength = midifile[track].size(); //get size of the file
}

void MidiReader:: getMidiCommands(MIDI_command commands[]){
    int idx = 0;
    for (int event=0; event<fileLength; event++) {
        commands[idx++] = {midifile[track][event].tick, (int)midifile[track][event][1]}; //get the ticks and the notes being played
    }
}
