/*#include <iostream>
#include <ctime>
#include <cmath>
#include "midiReader.h"

using namespace std;

void toggle(bool &a);
int arrayMultiply(bool a[], bool b[]);

int main() {
    //Skapa en ny midireader och läs in filen i commands
    MidiReader midireader = MidiReader("file.mid", 1);
    int length = midireader.fileLength;
    MIDI_command commands[length];
    midireader.getMidiCommands(commands);
    
    //Inställningar från midi-filen
    int bpm = 120;
    // int time_signature = 4;
    int ppq = 100;
    
    //Bestäm hur långt ett dt är och skriv om till klockcykler (behövs för clock)
    float dt = 60.00/(bpm*ppq);
    const float CLOCKS_PER_DT = dt*CLOCKS_PER_SEC;
    
    //Styr hur off spelaren kan vara men ändå få poäng
    // const int DT_MARGIN = 15;
    
    //Arrayer som håller koll på vilka toner som spelas, vilka som borde spelas och vilka som är tillgängliga för bonus
    bool playingNotes[96]= { 0 };
    bool bonusNotes[96]= { 0 };
    bool keyboardState[96]= { 0 };
    
    //För att testa, gör en färdig array med "spelar-input"
    MIDI_command playerInput[length];
    MIDI_command midiFile[length];
    for(int i=0; i < length; i++) {
        midiFile[i] = commands[i];
        playerInput[i] = commands[i];
    }
    //En placeholder för när spelaren får bonus, ska bytas ut mot midi
    MIDI_command bonusFile[4] = {{100, 1}, {110, 1}, {200, 2}, {210, 2}};
    
    
    //Tider som representerar när låten började och hur långt in vi är
    clock_t start_time;
    clock_t elapsed_time;
    int last_timestamp = 0;
    start_time = clock();
    
    int last_input = 0;
    int last_file_printed = 0;
    // int last_file_evaluated = 0;
    int last_bonus_printed = 0;
    
    float timestamp = 0;
    int score = 0;
    
    while (timestamp < 667) {
        //Räkna ut hur långt in i låten vi är
        elapsed_time = clock() - start_time;
        //Avrunda tiden till en timestamp, se rad 23
        timestamp = (float)elapsed_time/CLOCKS_PER_DT;
        
        //Om det är dags för nästa timestamp (loopen går många gånger per timestamp men vi vill bara köra koden en gång varje timestamp)
        if (timestamp > (last_timestamp + 1)) { //compare function thingy
            last_timestamp++;
            
            //Kolla om det finns några nya kommandon i midifilen som ska uppdateras
            while (round(timestamp)-1 == midiFile[last_file_printed].timestamp) {
                int key = midiFile[last_file_printed].key;
                toggle(playingNotes[key]);
                last_file_printed++;
            }
            //Kolla om det finns några nya bonusnoter som ska uppdateras
            while (round(timestamp)-1 == bonusFile[last_bonus_printed].timestamp) {
                int key = bonusFile[last_bonus_printed].key;
                toggle(bonusNotes[key]);
                last_bonus_printed++;
            }
            //Kolla om det finns någon ny input från spelaren
            while (round(timestamp)-1 == playerInput[last_input].timestamp) {
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
            //Ge 10 poäng för varje rätt tangent spelaren håller ner
            score += 10*arrayMultiply(keyboardState, playingNotes);
        }
    }
    
    cout << "Your score is: " << score << endl;
    
    return 0;
}

// Gör true till false och vice versa
void toggle(bool &a) {
    if (a) a = false;
    else a = true;
}

// Returnerar antal index som matchar i arrayerna, t.ex. [0,0,1] och [0,1,1] returnerar 1
int arrayMultiply(bool a[], bool b[]) {
    // a and b must be same length
    int result = 0;
    for (int i = 0; i < 72; i++) {
        if (a[i] && b[i]) result++;
    }
    return result;
}
*/
