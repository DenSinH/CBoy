#ifndef CBOY_CPU_H
#define CBOY_CPU_H

#include <stdint.h>
#include <stdbool.h>
#include "../MEM/MEM.h"

#define CLOCK_SPEED 4194304

typedef struct {
    bool Z: 1;     // zero flag
    bool N: 1;     // add/sub-flag
    bool H: 1;     // half carry flag
    bool C: 1;     // carry flag
    char unused: 4;
} s_flags;

typedef struct s_CPU {
    uint8_t A, B, C, D, E, H, L;
    uint16_t SP, PC;
    s_flags flags;

    void (*unprefixed[0x100])(struct s_CPU* cpu, uint8_t instruction);
    void (*prefixed[0x100])(struct s_CPU* cpu, uint8_t instruction);

    s_MEM* mem;

} s_CPU;

void step(s_CPU* cpu);
void init_cpu(s_CPU* cpu);

#endif //CBOY_CPU_H
