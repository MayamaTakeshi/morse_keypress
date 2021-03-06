This was my initial thought about how to play the tones.
I was going to insert instructions in a circular buffer and generates sampled based on them when portaudio asked for them.
However, in the middle of the coding, I realized it would be simpler to use a child thread that could sleep to control the duration of the tones.

