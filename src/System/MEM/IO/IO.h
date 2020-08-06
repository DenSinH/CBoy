#ifndef CBOY_IO_H
#define CBOY_IO_H

#include <stdint.h>

uint8_t IO_read(s_MEM* mem, uint16_t address);
void IO_write(s_MEM* mem, uint16_t address, uint8_t value);


#endif //CBOY_IO_H
