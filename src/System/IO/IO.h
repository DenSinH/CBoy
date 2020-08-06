#ifndef CBOY_IO_H
#define CBOY_IO_H

#include <stdint.h>

typedef struct s_IO {
    uint8_t registers[0x80];
} s_IO;

uint8_t IO_read(s_IO* mem, uint16_t address);
void IO_write(s_IO* mem, uint16_t address, uint8_t value);


#endif //CBOY_IO_H
