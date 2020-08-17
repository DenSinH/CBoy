#ifndef CBOY_PPU_H
#define CBOY_PPU_H

#include <stdint.h>

#include "../MEM/MEM.h"
#include "../IO/IO.h"
#include "sprites.h"

#define GB_WIDTH 160
#define GB_HEIGHT 144
#define GB_SCALE 3

#define MODE_2_DOTS 20
// this is just "on average"
#define MODE_3_DOTS MODE_2_DOTS + 57

typedef struct s_PPU {
    uint32_t display[GB_WIDTH * GB_HEIGHT];
    uint8_t* scanline;
    STAT_mode mode;

    s_sprite sprites[10];
    uint8_t number_of_sprites;

    uint8_t BG_pixels[GB_WIDTH];
    uint8_t sprite_pixels[GB_WIDTH];
    bool sprite_prio[GB_WIDTH];

    s_MEM* mem;
    s_IO* IO;
} s_PPU;

static inline void update_stat_mode(s_PPU* ppu, STAT_mode mode) {
    s_STAT stat = CAST_VAL_TO_REGISTER(s_STAT, ppu->IO->registers[reg_STAT]);
    ppu->mode = stat.ModeFlag = mode;
    ppu->IO->registers[reg_STAT] = CAST_REGISTER_TO_VAL(stat);

    if (mode < 3) {
        if ((ppu->IO->registers[reg_STAT] >> (3 + mode)) & 1) {
            request_interrupt(ppu->IO, Interrupt_LCDStat);
        }
    }
}

static inline void update_stat_line(s_PPU* ppu) {
    s_STAT stat = CAST_VAL_TO_REGISTER(s_STAT, ppu->IO->registers[reg_STAT]);
    stat.CoincidenceFlag = (*ppu->scanline) == ppu->IO->registers[reg_LYC];
    ppu->IO->registers[reg_STAT] = CAST_REGISTER_TO_VAL(stat);

    if (stat.CoincidenceFlag && stat.CoincidenceIRQ) {
        request_interrupt(ppu->IO, Interrupt_LCDStat);
    }
}

void do_scanline(s_PPU* ppu, s_MEM* mem);

#endif //CBOY_PPU_H
