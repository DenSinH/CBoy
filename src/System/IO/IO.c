#include "IO.h"
#include "../CPU/CPU.h"

#include "log.h"


void IO_init(s_IO* IO) {
    IO->registers[reg_JOYP] = 0xff;  // for unused bits
}

uint8_t IO_read(s_IO* IO, uint16_t address) {
    // log_debug("Read %02x from IO port %02x", IO->registers[address], address);
    switch (address) {
        case reg_JOYP:
            if (IO->registers[reg_JOYP] & JOYP_direction_keys) {
                return 0xc0 | (IO->registers[reg_JOYP] & 0x30) | direction_keys(&IO->joypad);
            }
            else if (IO->registers[reg_JOYP] & JOYP_button_keys) {
                // button keys
                return 0xc0 | (IO->registers[reg_JOYP] & 0x30) | button_keys(&IO->joypad);
            }
            else {
                // none selected
                return 0xcf | (IO->registers[reg_JOYP] & 0x30);
            }
        default:
            return IO->registers[address];
    }
}

void IO_write(s_IO* IO, uint16_t address, uint8_t value) {
    // log_debug("Wrote %c to IO port %02x", value, address);
    switch (address) {
        default:
            IO->registers[address] = value;
            break;
    }
}

