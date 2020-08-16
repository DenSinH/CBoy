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
        bool LCDDisplayEnable:      1;
    };
} s_LCDC;

typedef enum STAT_mode {
    mode_HBlank   = 0,
    mode_VBlank   = 1,
    mode_OAM      = 2,
    mode_transfer = 3
} STAT_mode;

typedef union s_STAT {
    struct {
        STAT_mode ModeFlag:         2;
        bool CoincidenceFlag:       1;
        bool HBlankIRQ:             1;
        bool VBlankIRQ:             1;
        bool OAMIRQ:                1;
        bool CoincidenceIRQ:        1;
    };
} s_STAT;

#endif //CBOY_LCD_H
