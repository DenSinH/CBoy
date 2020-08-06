#ifndef CBOY_LCD_H
#define CBOY_LCD_H

#include <stdbool.h>
#include <stdint.h>

// note to self: define bit 0 first, then down to bit 7

typedef union s_LCDC {
    struct {
        bool BGDisplay:             1;
        bool OBJDisplay:            1;
        bool OBJSize:               1;
        bool BGTileMapDisplay:      1;
        bool BGWindowTileData:      1;
        bool WindowDisplayEnable:   1;
        bool WindowTileMapDisplay:  1;
        bool LCDDisplayEnalbe:      1;
    };
} s_LCDC;

#endif //CBOY_LCD_H
