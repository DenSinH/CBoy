#ifndef CBOY_MEM_H
#define CBOY_MEM_H

#include <stdint.h>
#include "../IO/IO.h"

typedef struct {
    uint8_t ROM_b00[0x4000];        // 0000 - 3FFF
    uint8_t ROM_bNN[0x4000];        // 4000 - 7FFF
    uint8_t VRAM[0x2000];           // 8000 - 9FFF
    uint8_t eRAM[0x2000];           // A000 - BFFF
    uint8_t WRAM_b0[0x1000];        // C000 - CFFF
    uint8_t WRAM_bN[0x1000];        // D000 - DFFF
    uint8_t OAM[0xa0];              // FE00 - FE9F
    s_IO* IO;                       // FF00 - FF7F / IE at FFFF
    uint8_t HRAM[0x80];             // FF00 - FF7F
} s_MEM;


void load_rom(s_MEM* mem, char file_name[]);
void load_boot_rom(s_MEM* mem);

uint8_t read_byte(s_MEM* mem, uint16_t address);
uint16_t read_short(s_MEM* mem, uint16_t address);
void write_byte(s_MEM* mem, uint16_t address, uint8_t value);
void write_short(s_MEM* mem, uint16_t address, uint16_t value);

#endif //CBOY_MEM_H
