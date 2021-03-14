#include <stdio.h>
#include <math.h>
#include <unistd.h> // for usleep()

#include <stdexcept>

#include <functional>

#include "audioplayer.hpp"

#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)



/* https://stackoverflow.com/questions/19808054/convert-c-function-pointer-to-c-function-pointer */
template <typename T>
struct Callback;

template <typename Ret, typename... Params>
struct Callback<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
};

// Initialize the static member.
template <typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::func;


int AudioPlayer::paCallback(const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    //printf("paCallback\n");
    float *out = (float*)outputBuffer;
    unsigned long i;
    uint32_t freq = data.freq;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    for( i=0; i<framesPerBuffer; i++ )
    {
        if(data.freq == 0) {
            *out++ = 0;
            *out++ = 0;
            continue;
        }
        data.total_count++;

        int overflow_max = SAMPLE_RATE / freq;
        uint32_t data_cnt = data.counter % overflow_max;
        if(data_cnt > overflow_max/2) {
            *out++ = 0xff;
            *out++ = 0xff;
        } else {
            *out++ = 0x00;
            *out++ = 0x00;
        }
        data.counter++;
    }

    return paContinue;
}

AudioPlayer::AudioPlayer() {
    PaStreamParameters outputParameters;

    PaError err;
    int i;

    // Store member function and the instance using std::bind.
    Callback<int(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*)>::func = std::bind(&AudioPlayer::paCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);

    // Convert callback-function to c-pointer.
    int (*c_func)(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*) = static_cast<decltype(c_func)>(Callback<int(const void*, void*, unsigned long, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void*)>::callback);

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
        &stream,
        NULL, /* no input */
        &outputParameters,
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        paClipOff,      /* we won't output out of range samples so don't bother clipping them */
        c_func,
        NULL);

    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    return;

error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    exit(1);
}

AudioPlayer::~AudioPlayer() {
    PaError err = 0;
    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();

    return;

error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    exit(1);
}


void AudioPlayer::set_freq(int frequency)
{
    data.up_count = 0; // do not stop!
    data.freq = frequency;
}

