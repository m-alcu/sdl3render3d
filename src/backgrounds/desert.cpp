#include <iostream>
#include "desert.hpp"

void Desert::calcPalette(uint32_t *palette) {

    uint16_t r_i = 0x2d80 * 4;
    uint16_t g_i = 0x2380 * 4;
    uint16_t b_i = 0x2d80 * 4;

    uint16_t r_dif = 0x0074 * 4;
    uint16_t g_dif = 0x0064 * 4;
    uint16_t b_dif = 0x0074 * 4;

    for (int p=0; p<64; p++) {
        uint8_t r = static_cast<uint8_t>(r_i >> 8);
        uint8_t g = static_cast<uint8_t>(g_i >> 8);
        uint8_t b = static_cast<uint8_t>(b_i >> 8);
        uint8_t a = 0xff; 
        palette[p] = (a << 24) | (r << 16) | (g << 8) | b;
        r_i -= r_dif;
        g_i -= g_dif;
        b_i -= b_dif;
    }
}


void Desert::draw(uint32_t *pixels, uint16_t height, uint16_t width) {

    if (!getNeedsUpdate()) {
        return; // No need to update if not required
    }


    seed1 = 0x1234;
    seed2 = 0x2293;
    uint8_t* greys = new uint8_t[width * height];

    uint32_t* desertPalette = new uint32_t[64];
    Desert::calcPalette(desertPalette);

    const uint8_t desertBase[40] = { 
        15,15,16,16,17,19,21,23,26,29,31,31,31,35,39,42,45,43,60,57,
        55,51,47,45,44,42,39,37,34,31,29,25,21,19,16,16,16,15,15,15  
    };

    for(int point=0; point<width; point++) {
        uint8_t grey = desertBase[point % sizeof(desertBase)];
        greys[point] = grey;
        pixels[point] = desertPalette[grey];
    }

    for (int point=width; point<(width*height - width); point++) {
        uint8_t h_point = greys[point - width + 1];
        uint8_t l_point = greys[point - width];
        uint8_t grey = (h_point >> 1) + (h_point >> 2) + (h_point >> 3) + (l_point >> 3);

        if (seed1 >= 0x8000) {
            seed1 += seed1;
            seed1 ^= seed2;
        } else {
            seed1 += seed1;
        }
        grey += (uint8_t) ((seed1 & 0x00ff) >> 6);
        greys[point] = grey;
        pixels[point] = desertPalette[grey];
    }

    setNeedsUpdate(false);

    delete[] desertPalette;
    delete[] greys;

}