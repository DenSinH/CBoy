#include "IO.h"
#include "../CPU/CPU.h"

#include "log.h"


uint8_t IO_read(s_IO* IO, uint16_t address) {
    // log_debug("Read %02x from IO port %02x", IO->registers[address], address);
    if (address == reg_LY) {
        return 0x90;
    }
    return IO->registers[address];
}

void IO_write(s_IO* IO, uint16_t address, uint8_t value) {
    // log_debug("Wrote %c to IO port %02x", value, address);
    IO->registers[address] = value;
}

