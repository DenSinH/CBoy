#ifndef CBOY_PPU_H
#define CBOY_PPU_H

#include <stdint.h>

#include "../MEM/MEM.h"
#include "../IO/IO.h"

#define GB_WIDTH 160
#define GB_HEIGHT 144
#define GB_SCALE 3

typedef struct s_PPU {
    uint32_t display[GB_WIDTH * GB_HEIGHT];
    uint8_t* scanline;

    s_MEM* mem;
    s_IO* IO;
} s_PPU;

void do_scanline(s_PPU* ppu, s_MEM* mem);

#endif //CBOY_PPU_H
