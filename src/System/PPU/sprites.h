#ifndef CBOY_SPRITES_H
#define CBOY_SPRITES_H

#include <stdint.h>
#include <stdbool.h>

typedef union s_attributes {
    struct {
        uint8_t PaletteNumCGB:     3;
        uint8_t VRAMBank:          1;
        uint8_t PaletteNumNonCGB:  1;
        bool Xflip:                1;
        bool Yflip:                1;
        bool priority:             1;
    };
} s_attributes;

#define CAST_ATTRIBUTES(byte) *(s_attributes*)&byte

typedef struct s_sprite {
    uint8_t y;
    uint8_t x;
    uint8_t Tid;
    s_attributes attributes;
} s_sprite;


#endif //CBOY_SPRITES_H
