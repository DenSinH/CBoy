#include "../../CPU/CPU.h"

#include "log.h"


uint8_t IO_read(s_MEM* mem, uint16_t address) {
    // log_debug("Read from IO port %02x", address);
    return mem->IO[address];
}

void IO_write(s_MEM* mem, uint16_t address, uint8_t value) {
    log_debug("Wrote %c to IO port %02x", value, address);
    mem->IO[address] = value;
}

