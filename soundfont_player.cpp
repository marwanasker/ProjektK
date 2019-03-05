#define TSF_IMPLEMENTATION
#include "tsf.h"

#include "minisdl_audio.h"

// Holds the global instance pointer
static tsf* g_TinySoundFont;

// A Mutex so we don't call note_on/note_off while rendering audio samples

// Callback function called by the audio thread
static void AudioCallback(void* data, Uint8 *stream, int len)
{
    // Note we don't do any thread concurrency control here because in this
    // example all notes are started before the audio playback begins.
    // If you do play notes while the audio thread renders output you
    // will need a mutex of some sort.
    int SampleCount = (len / (2 * sizeof(short))); //2 output channels
    tsf_render_short(g_TinySoundFont, (short*)stream, SampleCount, 0);
}


void play(int pitch, int duration, tsf* soundfont) {
    // Define the desired audio output format we request
    SDL_AudioSpec OutputAudioSpec;
    OutputAudioSpec.freq = 44100;
    OutputAudioSpec.format = AUDIO_S16;
    OutputAudioSpec.channels = 2;
    OutputAudioSpec.samples = 4096;
    OutputAudioSpec.callback = AudioCallback;
    
    // Initialize the audio system
    if (SDL_AudioInit(NULL) < 0)
    {
        fprintf(stderr, "Could not initialize audio hardware or driver\n");
    }
    
    // Load the SoundFont from the memory block
    g_TinySoundFont = soundfont;
    if (!g_TinySoundFont)
    {
        fprintf(stderr, "Could not load soundfont\n");
    }
    
    // Set the rendering output mode to 44.1khz and -10 decibel gain
    tsf_set_output(g_TinySoundFont, TSF_STEREO_INTERLEAVED, OutputAudioSpec.freq, -10);
    
    // Start the note before starting the audio playback
    tsf_note_on(g_TinySoundFont, 0, pitch, 1.0f); //C2
    
    // Request the desired audio output format
    if (SDL_OpenAudio(&OutputAudioSpec, NULL) < 0)
    {
        fprintf(stderr, "Could not open the audio hardware or the desired audio output format\n");
    }
    
    // Start the actual audio playback here
    // The audio thread will begin to call our AudioCallback function
    SDL_PauseAudio(0);
    
    // Let the audio callback play some sound for 3 seconds
    SDL_Delay(duration);
    
    tsf_note_off(g_TinySoundFont, 0, pitch);
    
}
