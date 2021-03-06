#include "portaudio.h"
#include <stdint.h>

typedef struct
{
    uint32_t total_count;
    uint32_t up_count;

    uint32_t counter;
    uint32_t prev_freq;
    uint32_t freq;
} paData;


class AudioPlayer {
private:
    PaStream *stream;
    paData data;

public:
    AudioPlayer();
    ~AudioPlayer();

    int paCallback(const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );

    void set_freq(int);
};
