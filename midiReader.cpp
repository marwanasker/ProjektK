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

class MidiReader{
public:
    MidiReader(string fileName, int track, int argc, char** argv);
    ~MidiReader();
    void getMidiCommands(MIDI_command commands[]);

private:
    Options options;
    MidiFile* midifile;
    int fileLength;
    int track;
};

MidiReader:: MidiReader(string fileName, int track, int argc, char** argv){
    options.process(argc, argv);
    if (options.getArgCount() == 0) midifile->read(fileName);
    else midifile->read(options.getArg(1));
    midifile->doTimeAnalysis();
    midifile->linkNotePairs();
    fileLength = midifile[1].size();
}

MidiReader :: ~MidiReader(){
    delete midifile;
}


void MidiReader:: getMidiCommands(MIDI_command commands[]){
    int tracks = midifile->getTrackCount();
    int idx = 0;
    for (int track=0; track <tracks; track++) {
        for (int event=0; event<midifile[track].size(); event++) {
            commands[idx++] = {(*midifile)[track][event].tick, (int)(*midifile)[track][event][1]};
        }
    }
}
