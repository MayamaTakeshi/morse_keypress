#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "keywatcher.hpp"

KeyWatcher::KeyWatcher() {
    display_name = XOpenDisplay(NULL);
    last_ongoing_key = 0;
}

KeyWatcher::~KeyWatcher() {
    XCloseDisplay(display_name);
}

char KeyWatcher::check() {
    char keys_return[32];

    bool any_pressed = false;

    XQueryKeymap(display_name,  keys_return);
    for (int i=0; i<32; i++) {
        if (keys_return[i] != 0) {
            int pos = 0;
            int num = keys_return[i];
            while (pos < 8) {
                if ((num & 0x01) == 1) {
                    KeySym key = NoSymbol;
                    u_int32_t keyCode = i*8+pos;
                    key = XkbKeycodeToKeysym(display_name, keyCode, 0, 0);
                    //printf("key=%c\n", key);

                    if(key) {
                        if(key != last_ongoing_key) {  
                            last_ongoing_key = key;
                            return key;
                        } else {
                            // if key is the same as last_ongoing then the keyup didn't happen. So check other keys
                        }

                        any_pressed = true;
                    }
                }
                pos++; num /= 2;
            }
        }
    }

    if(!any_pressed) {
        last_ongoing_key = 0;
    }
        
    return 0;
}

