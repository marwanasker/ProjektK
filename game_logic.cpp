#include <iostream>
#include <ctime>
#include <cmath>
#include "midiReader.h"

using namespace std;

void toggle(bool &a);
int arrayMultiply(bool a[], bool b[]);

int main(int argc, char** argv) {
   // int midi_length = getMidiLength(argc, argv);
 //   MIDI_command* result = new MIDI_command[midi_length];
 //   cout << "length is: " << midi_length << endl;
    MidiReader midireader =   MidiReader("twinkle.txt", 1, argc, argv);
    
    int bpm = 120;
    int time_signature = 4;
    int ppq = 100;

    float dt = 60.00/(bpm*ppq);
    const float CLOCKS_PER_DT = dt*CLOCKS_PER_SEC;

    const int DT_MARGIN = 15;

    bool playingNotes[72]= { 0 };
    bool bonusNotes[72]= { 0 };
    bool keyboardState[72]= { 0 };

    MIDI_command playerInput[3] = {{100, 1}, {200,1}, {220, 2}};
    MIDI_command midiFile[4] = {{100, 1}, {200,1}, {200, 2}, {300, 2}};
    MIDI_command bonusFile[4] = {{100, 1}, {110, 1}, {200, 2}, {210, 2}};
    
 /*   for (int i = 0; i < midi_length; i++) {
        cout << result[i].timestamp << " " << result[i].key << endl;
    }
*/
    clock_t start_time;
    clock_t elapsed_time;
    int last_timestamp = 0;
    start_time = clock();

    int last_input = 0;
    int last_file_printed = 0;
    int last_file_evaluated = 0;
    int last_bonus_printed = 0;

    float timestamp = 0;
    int score = 0;

    while (timestamp < 500) {
        elapsed_time = clock() - start_time;
        timestamp = (float)elapsed_time/CLOCKS_PER_DT;

        if (timestamp > (last_timestamp + 1)) {
            last_timestamp++;

            while (round(timestamp) == midiFile[last_file_printed].timestamp) {
                int key = midiFile[last_file_printed].key;
                toggle(playingNotes[key]);
                last_file_printed++;
            }

            while (round(timestamp) == bonusFile[last_bonus_printed].timestamp) {
                int key = bonusFile[last_bonus_printed].key;
                toggle(bonusNotes[key]);
                last_bonus_printed++;
            }

            while (round(timestamp) == playerInput[last_input].timestamp) {
                int key = playerInput[last_input].key;
                // Check bonus
                if (bonusNotes[key]) {
                    score += 500;
                    bonusNotes[key] = false;
                    cout << "Bonus!" << endl;
                    last_bonus_printed++;
                }

                toggle(keyboardState[key]);
                last_input++;
            }

            score += 10*arrayMultiply(keyboardState, playingNotes);
        }
    }

cout << "Your score is: " << score << endl;

return 0;
}

void toggle(bool &a) {
    if (a) a = false;
    else a = true;
}

int arrayMultiply(bool a[], bool b[]) {
    // a and b must be same length
    int result = 0;
    for (int i = 0; i < 72; i++) {
        if (a[i] && b[i]) result++;
    }
    return result;
}

