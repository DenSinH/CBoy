#ifndef CBOY_IO_H
#define CBOY_IO_H

#include <stdint.h>

#include "joypad.h"
#include "LCD.h"

#define IO_BASE 0xff00

#define CAST_REGISTER(type, value) *(type*)&value
#define CAST_UINT8_T(reg) *(uint8_t*)&reg

enum IO_regs {
    reg_JOYP    = 0x00,
    reg_LCDC    = 0x40,
    reg_LY      = 0x44,
    reg_BGP     = 0x47
} IO_regs;

typedef struct s_IO {
    uint8_t registers[0x80];

    s_JOYPAD joypad;
} s_IO;

void IO_init(s_IO* IO);
uint8_t IO_read(s_IO* IO, uint16_t address);
void IO_write(s_IO* IO, uint16_t address, uint8_t value);


#endif //CBOY_IO_H
