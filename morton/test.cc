#include <cstdio>
#include "morton.h"

int main() {
    // test morton2
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++)
            printf("%d\t", morton2(x,y));
        printf("\n");
    }
    // test morton3
    for (int x = 0; x < 256; x++) {
        printf("interleave3(0x%x): 0x%x\n", x, interleave3(x));
    }
    for (int z = 0; z < 4; z++) {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++)
                printf("%x\t", morton3(x, y, z));
            printf("\n");
        }
        printf("\n");
    }
}
