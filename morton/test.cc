#include <cstdio>
#include <cassert>
#include "morton.h"

void printBit(unsigned int n) {
    if (!n) {
        printf("0");
        return;
    }
    while (n) {
        if (n & 1)
            printf("1");
        else
            printf("0");

        n >>= 1;
    }
}

int main() {
    // test morton2
    for (int x = 0; x < 256; x++) {
        unsigned int i = interleave2(x);
        printf("interleave2(0x%02x) = 0x%04x, ", x, i);
        printBit(x); printf("\t"); printBit(i);
        printf("\n");
    }
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            printf("%u\t", morton2(x,y));
            assert(morton2(x,y) == morton2_64(x, y));
        }
        printf("\n");
    }

    // test morton3
    for (int x = 0; x < 256; x++) {
        unsigned int i = interleave3(x);
        printf("interleave3(0x%02x) = 0x%06x, ", x, i);
        printBit(x); printf("\t"); printBit(i);
        printf("\n");
    }
    for (int z = 0; z < 4; z++) {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++)
                printf("%u\t", morton3(x, y, z));
            printf("\n");
        }
        printf("\n");
    }
}
