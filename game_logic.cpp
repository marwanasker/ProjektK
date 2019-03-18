#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <cstdlib>
#include "midiReader.h"
#include "rtmidi.h"


using namespace std;

bool Keys[127];


void toggle(bool &a);
int arrayMultiply(bool a[], bool b[]);


void usage(void) {
	// Error function in case of incorrect command-line
	// argument specifications.
	std::cout << "\nuseage: cmidiin <port>\n";
	std::cout << "    where port = the device to use (default = 0).\n\n";
	exit(0);
}

void mycallback(double deltatime, std::vector< unsigned char > *message, void */*userData*/)
{
	unsigned int nBytes = message->size()-1;
	for (unsigned int i = 0; i<nBytes; i++)
		std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
	if (nBytes > 0)
		std::cout << "stamp = " << deltatime << std::endl;

	if ((int)message->at(0) == 144) {
		std::cout << (int)message->at(0) << std::endl;
		Keys[(int)message->at(1)] = true;
	}
	else {
		Keys[(int)message->at(1)] = false;
	}

	for (int i = 0; i < 127; i++) {
		std::cout << Keys[i];
	}
}


// This function should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.
bool chooseMidiPort(RtMidiIn *rtmidi);



int main(int argc, char ** /*argv[]*/){

    
    //Skapa en ny midireader och läs in filen i commands
    MidiReader midireader = MidiReader("file.mid", 1);
    vector<MIDI_properties> properties(midireader.fileLength);
    midireader.getMidiProperties(properties);
	
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
    bool playingNotes[109]= {0};
    bool bonusNotes[109]= {0};
    bool keyboardState[109]= {0};

	RtMidiIn *midiin = 0;

	// Minimal command-line check.
	if (argc > 2) usage();

	try {

		// RtMidiIn constructor
		midiin = new RtMidiIn();

		// Call function to select port.
		if (chooseMidiPort(midiin) == false) goto cleanup;

		// Set our callback function.  This should be done immediately after
		// opening the port to avoid having incoming messages written to the
		// queue instead of sent to the callback function.
		midiin->setCallback(&mycallback);

		// Don't ignore sysex, timing, or active sensing messages.
		midiin->ignoreTypes(false, false, false);

		std::cout << "\nReading MIDI input ... press <enter> to quit.\n";

		
		char input;
		std::cin.get(input);

	}
	catch (RtMidiError &error) {
		error.printMessage();
	}

cleanup:

	delete midiin;
	
    
    //För att testa, gör en färdig array med "spelar-input"
    vector<MIDI_properties> playerInput(properties.size());
    vector<MIDI_properties> fileInput(properties.size());
    //MIDI_properties playerInput[properties.size()];
   // MIDI_properties midiFile[properties.size()];
    
   for(int i=0; i < properties.size(); i++) {
        fileInput[i] = properties[i];
        playerInput[i] = properties[i];
    }
    
    //En placeholder för när spelaren får bonus, ska bytas ut mot midi
    MIDI_properties bonusFile[4] = {{100, 1}, {110, 1}, {200, 2}, {210, 2}};
    
    
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
    
    while (timestamp < 120) {
        //Räkna ut hur långt in i låten vi är
        elapsed_time = clock() - start_time;
        //Avrunda tiden till en timestamp, se rad 23
        timestamp = (float)elapsed_time/CLOCKS_PER_DT;
        
        //Om det är dags för nästa timestamp (loopen går många gånger per timestamp men vi vill bara köra koden en gång varje timestamp)
        if (timestamp > (last_timestamp + 1)) { //compare function thingy
            last_timestamp++;
            
            //Kolla om det finns några nya kommandon i midifilen som ska uppdateras
            while (round(timestamp)-1 == fileInput[last_file_printed].start) {
                int key = fileInput[last_file_printed].note;
                toggle(playingNotes[key]);
                last_file_printed++;
            }
             //Kolla om det finns några nya bonusnoter som ska uppdateras
            while (round(timestamp)-1 == bonusFile[last_bonus_printed].start) {
                int key = bonusFile[last_bonus_printed].note;
                toggle(bonusNotes[key]);
                last_bonus_printed++;
            }
            //Kolla om det finns någon ny input från spelaren
            while (round(timestamp)-1 == playerInput[last_input].start) {
                int key = playerInput[last_input].note;
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

bool chooseMidiPort(RtMidiIn *rtmidi)
{
	std::cout << "\nWould you like to open a virtual input port? [y/N] ";

	std::string keyHit;
	std::getline(std::cin, keyHit);
	if (keyHit == "y") {
		rtmidi->openVirtualPort();
		return true;
	}

	std::string portName;
	unsigned int i = 0, nPorts = rtmidi->getPortCount();
	if (nPorts == 0) {
		std::cout << "No input ports available!" << std::endl;
		return false;
	}

	if (nPorts == 1) {
		std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
	}
	else {
		for (i = 0; i<nPorts; i++) {
			portName = rtmidi->getPortName(i);
			std::cout << "  Input port #" << i << ": " << portName << '\n';
		}

		do {
			std::cout << "\nChoose a port number: ";
			std::cin >> i;
		} while (i >= nPorts);
		std::getline(std::cin, keyHit);  // used to clear out stdin
	}

	rtmidi->openPort(i);

	return true;
}
