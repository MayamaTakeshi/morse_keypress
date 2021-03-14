#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <poll.h>
#include <memory.h>

#include "keywatcher.hpp"
#include "morse.hpp"
#include "audioplayer.hpp"


int wait_n_read(int fd, char* buf, int szBuf) {
    struct pollfd pfd = {
        .fd      = fd,
        .events  = POLLIN,
        .revents = 0
    };
    poll(&pfd, 1, -1); //wait for an event
    int ret = read(fd, buf, szBuf);
    if (ret == -1) {
        perror("In read()");
    }
    return ret;
}

#define FREQ 880
#define DOT_DURATION 40

void msleep(int d){
    usleep(d*1000);
}

void * child(void *c) {
    int chan = *(int*)c;
    //printf("child chan=%i\n", chan);

    Morse m = Morse();

    AudioPlayer ap = AudioPlayer();
    ap.set_freq(0);

    char buf[256];
    while(1) {
        //printf("child waiting\n");
        memset(buf, 0, sizeof(buf));
        if (wait_n_read(chan, buf, sizeof(buf)-1) == -1) {
            printf("wait_n_read error\n");
        } else {
            //printf("child got %s\n", (char *)buf);

            int length = strlen(buf);
            for(int i=0 ; i<length ; i++) {
                const char *s = m.fromAsc(buf[i]);
                if(!s[0]) continue;
                //printf("child playing %c %s\n", buf[i], (char *)s);
                printf(" Playing %c %s\n", buf[i], (char *)s);

                int siglength = strlen(s);
                for(int j=0 ; j<siglength ; j++) {
                    ap.set_freq(FREQ);

                    if(s[j] == '.') {
                        msleep(DOT_DURATION);                    
                    } else {
                        msleep(DOT_DURATION * 3);                    
                    }

                    ap.set_freq(0);
                    if(j < siglength-1) {
                        //interval between dots and dashes in the same letter
                        msleep(DOT_DURATION);
                    } else {
                        //interval between letters
                        msleep(DOT_DURATION * 3);
                    }
                }
            }
        }
        //printf("child waiting ended. looping\n");
    }
}


int main() {
    KeyWatcher kw = KeyWatcher();
    int chan[2];

    if (pipe(&chan[0]) == -1) {
        perror("In pipe");

    }


    pthread_t tid;
    pthread_create(&tid, NULL, child, (void*)&chan[0]);
    printf("main chan=%i\n", chan[1]);

    while(1) {
        char key = kw.check();
        if(key) {
            char buf[16];
            sprintf(buf, "%c", key);
            write(chan[1], buf, strlen(buf));
            //printf("written to chan %i\n", chan[1]);
        }
        usleep(30000);
    }
}
