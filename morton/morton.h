#pragma once

// from http://graphics.stanford.edu/~seander/bithacks.html

inline
unsigned short morton2_64(unsigned char x, unsigned char y) {
    // Interleave bits of (8-bit) x and y, so that all of the
    // bits of x are in the even positions and y in the odd;
    unsigned short z; // z gets the resulting 16-bit Morton Number.
    z = (((x * 0x0101010101010101ULL & 0x8040201008040201ULL) * 
            0x0102040810204081ULL >> 49) & 0x5555) |
        (((y * 0x0101010101010101ULL & 0x8040201008040201ULL) * 
         0x0102040810204081ULL >> 48) & 0xAAAA);
    return z;
}

inline
unsigned int interleave2(unsigned short u) {
    unsigned int x = u;
    x = (x | (x << 8)) & 0x00ff00ff;
    x = (x | (x << 4)) & 0x0f0f0f0f;
    x = (x | (x << 2)) & 0x33333333;
    x = (x | (x << 1)) & 0x55555555;
    return x;
}

inline
unsigned int morton2(unsigned int x, unsigned int y) {
    return interleave2(x) | interleave2(y) << 1;
}

inline
unsigned int interleave3(unsigned short u) {
    unsigned int x = u;
    x = (x | (x << 16)) & 0xff0000ff;
    x = (x | (x << 8 )) & 0x0f00f00f;
    x = (x | (x << 4 )) & 0xc30c30c3;
    x = (x | (x << 2 )) & 0x49249249;
    return x;
}

inline
unsigned int morton3(unsigned int x, unsigned int y, unsigned int z) {
    // Interleave lower 11 bits of x, y and z.
    return interleave3(x) | interleave3(y) << 1 | interleave3(z) << 2;
}
