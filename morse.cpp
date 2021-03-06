#include <cstdio>
#include "morse.hpp"

char *Morse::fromAsc(char c) {
    char *p = ascii;
    while(*p) {
        if(*p == c) {
            int idx = p-ascii;
            printf("idx=%d\n", idx);
            return morse[idx];
        }
        p++;
    }
    return (char*)"";
}
