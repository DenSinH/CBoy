#ifndef CBOY_CPU_H
#define CBOY_CPU_H

#include <stdint.h>
#include <stdbool.h>

#define CLOCK_SPEED 4194304

typedef struct {
    bool zf: 1;     // zero flag
    bool n: 1;      // add/sub-flag
    bool h: 1;      // half carry flag
    bool cy: 1;     // carry flag
    char unused: 4;
} s_flags;

typedef struct {
    uint8_t A, B, C, D, E, H, L;
    uint16_t SP, PC;
    s_flags flags;
} s_Z80;

#endif //CBOY_CPU_H
