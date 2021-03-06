# morse_keypress

## Overview

This is a simple app that I wrote for an experiment:
  To try to memorize morse code just by listening to dot/dash audio generated when keys are pressed in the keyboard during normal work (coding etc).
  So, basically when we press a key like 'a' we will listen to '.-' (audio feedback).
  Of course this feedback might not be good if you are a fast typist (but I'm not).

Obs: it was coded on Debian 10 (it will not work on Windows etc).

## Compiling

Install lib dependencies:
```
apt install libxcb-util0-dev lxcb-xtest0 portaudio19-dev

```
Then build it:
```
make
```

## Usage

Just start it:
```
./morse_keypress
```

and do some typing. It can be in the same shell or in any other shell, browser etc in your X system.

