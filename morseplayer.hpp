#include "portaudio.h"

struct Item {
    bool in_use;
	unsigned samples;
	unsigned freq;
	Item *next;
};

#define MAX_ITEMS 256


class MorsePlayer {
private:
    PaStream *stream;
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
