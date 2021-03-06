#include "portaudio.h"
#include <stdint.h>

struct Item {
    bool in_use;
	unsigned samples;
	unsigned freq;
	Item *next;
};

#define MAX_ITEMS 256

typedef struct
{
    uint32_t total_count;
    uint32_t up_count;

    uint32_t counter;
    uint32_t prev_freq;
    uint32_t freq;
} paData;


class MorsePlayer {
private:
    PaStream *stream;
    paData data;

    Item *first = NULL;
    Item *insertion_pointer;
    Item *consumption_pointer;

    void prepareCircularBuffer();

public:
    MorsePlayer();
    ~MorsePlayer();

    int paCallback(const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );

    void add(int samples, int freq);
    int consume(void *outputBuffer, unsigned long framesPerBuffer);
    void set_freq(int);
};
