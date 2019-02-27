#include "MidiFile.h"
#include "Options.h"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace smf;

struct MIDI_command {
    int timestamp;
    int key;
} ;

int getMidiLength(int argc, char** argv) {
    Options options;
    options.process(argc, argv);
    MidiFile midifile;
    if (options.getArgCount() == 0) midifile.read("twinkle.txt");
    else midifile.read(options.getArg(1));
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();
    
    return midifile[1].size();
}

void readMidiFile(int argc, char** argv, MIDI_command result[]){
    Options options;
    options.process(argc, argv);
    MidiFile midifile;
    if (options.getArgCount() == 0) midifile.read("twinkle.txt");
    else midifile.read(options.getArg(1));
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();

    int idx = 0;
    
    int tracks = midifile.getTrackCount();
    //cout << "TPQ: " << midifile.getTicksPerQuarterNote() << endl;
    //if (tracks > 1) cout << "TRACKS: " << tracks << endl;
    for (int track=1; track<2; track++) { //Only print second track!
        //if (tracks > 1) cout << "\nTrack " << track << endl;
        //cout << "Tick\tSeconds\tDur\tMessage" << endl;
        for (int event=0; event<midifile[track].size(); event++) {
            /*cout << dec << midifile[track][event].tick;
            cout << '\t' << dec << midifile[track][event].seconds;
            cout << '\t';
            if (midifile[track][event].isNoteOn())
                cout << midifile[track][event].getDurationInSeconds();
            cout << '\t' << hex;
            for (int i=0; i<midifile[track][event].size(); i++)
                cout << (int)midifile[track][event][i] << ' ';
            cout << endl; */
            result[idx++] = {midifile[track][event].tick, (int)midifile[track][event][1]};
        }
    }
}
