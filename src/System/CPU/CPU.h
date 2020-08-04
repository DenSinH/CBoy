#ifndef CBOY_CPU_H
#define CBOY_CPU_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../MEM/MEM.h"

#include "log.h"

#define CLOCK_SPEED 4194304

typedef union s_flags {
    struct {
        bool Z: 1;     // zero flag
        bool N: 1;     // add/sub-flag
        bool H: 1;     // half carry flag
        bool C: 1;     // carry flag
        char unused: 4;
    };
    uint8_t F;
} s_flags;

typedef enum {
    B = 0,
    C = 1,
    D = 2,
    E = 3,
    H = 4,
    L = 5,
    /* HL = 6 (16 bit register, separate case)*/
    A = 7
} r8;

typedef enum {
    BC = 0,
    DE = 2,
    HL = 4,
    SP = 6
} r16;

typedef struct s_CPU {
    uint8_t registers[8];
    uint16_t SP, PC;
    s_flags flags;

    void (*unprefixed[0x100])(struct s_CPU* cpu, uint8_t instruction);
    void (*prefixed[0x100])(struct s_CPU* cpu, uint8_t instruction);

    s_MEM* mem;

} s_CPU;

static inline uint16_t get_r16(s_CPU* cpu, r16 index) {
    /* get 16 bit register from cpu based on index */
    if (index == SP)
        return cpu->SP;
    return (uint16_t)(cpu->registers[index] | (cpu->registers[index + 1] << 8));
}

static inline void set_r16(s_CPU* cpu, r16 index, uint16_t value) {
    /* set 16 bit register from cpu based on index */
    if (index == SP) {
        cpu->SP = value;
    }
    else {
        cpu->registers[index]     = (uint8_t)(value & 0xff);
        cpu->registers[index + 1] = (uint8_t)(value >> 8);
    }
}

static inline void mGBA_log_format(s_CPU* cpu, char dest[69]) {
    /* format the cpu's information into a char[] like mGBA does */
    if (
        sprintf(
            dest, "A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X SP: %04X PC: 00:%04X",
            cpu->registers[A],
            cpu->flags.F,
            cpu->registers[B],
            cpu->registers[C],
            cpu->registers[D],
            cpu->registers[E],
            cpu->registers[H],
            cpu->registers[L],
            cpu->SP,
            cpu->PC
        ) == -1) {

        log_fatal("Couldn't format CPU string");
    }
}

void step(s_CPU* cpu);
void init_cpu(s_CPU* cpu);

#endif //CBOY_CPU_H
