#ifndef CBOY_CPU_H
#define CBOY_CPU_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "../MEM/MEM.h"
#include "../IO/IO.h"

#include "log.h"

#define GB_CLOCK_SPEED 4194304

typedef enum {
    r8_B = 0,
    r8_C = 1,
    r8_D = 2,
    r8_E = 3,
    r8_H = 4,
    r8_L = 5,
    r8_HL = 6,
    r8_A = 7
} e_r8;

typedef enum {
    r16_BC = 0,
    r16_DE = 2,
    r16_HL = 4,
    r16_SP = 6
} e_r16;

typedef enum {
    flag_Z = 0x80,
    flag_N = 0x40,
    flag_H = 0x20,
    flag_C = 0x10
} flag;

typedef struct s_CPU {
    uint8_t registers[8];
    uint16_t SP, PC;
    uint8_t flags;
    bool IME;
    bool freeze;

    int (*unprefixed[0x100])(struct s_CPU* cpu, uint8_t instruction);
    int (*prefixed[0x100])(struct s_CPU* cpu, uint8_t instruction);

    s_MEM* mem;
    s_IO* IO;

} s_CPU;

static inline uint8_t get_r8(s_CPU* cpu, e_r8 index) {
    if (index == r8_HL)
        return read_byte(cpu->mem, (uint16_t)(cpu->registers[index + 1] | (cpu->registers[index] << 8)));
    return cpu->registers[index];
}

static inline void set_r8(s_CPU* cpu, e_r8 index, uint8_t value) {
    if (index == r8_HL)
        write_byte(cpu->mem, (uint16_t)(cpu->registers[index + 1] | (cpu->registers[index] << 8)), value);
    else
        cpu->registers[index] = value;
}

static inline uint16_t get_r16(s_CPU* cpu, e_r16 index) {
    /* get 16 bit register from cpu based on index */
    if (index == r16_SP)
        return cpu->SP;
    // basically, r8's are one big r16 together, so "endianness" is reversed
    return (uint16_t)(cpu->registers[index + 1] | (cpu->registers[index] << 8));
}

static inline void set_r16(s_CPU* cpu, e_r16 index, uint16_t value) {
    /* set 16 bit register from cpu based on index */
    if (index == r16_SP) {
        cpu->SP = value;
    }
    else {
        cpu->registers[index + 1] = (uint8_t)(value & 0xff);
        cpu->registers[index ]    = (uint8_t)(value >> 8);
    }
}

#define HALF_CARRY_8BIT_SUB(op1, op2) ((op1 & 0xF) < (op2 & 0xF))
#define HALF_CARRY_8BIT_SUB_C(op1, op2, C) ((op1 & 0xF) < ((op2 & 0xF) + C))
#define HALF_CARRY_16BIT_SUB(op1, op2) ((op1 & 0xFFF) < (op2 & 0xFFF))
#define HALF_CARRY_8BIT_ADD(op1, op2) (((op1 & 0xF) + (op2 & 0xF)) & 0x10)
#define HALF_CARRY_8BIT_ADD_C(op1, op2, C) (((op1 & 0xF) + (op2 & 0xF) + C) & 0x10)
#define HALF_CARRY_16BIT_ADD(op1, op2) (((op1 & 0xFFF) + (op2 & 0xFFF)) & 0x1000)

// half carry is the result of the last adder operation, so for 16 bit the mask is 0xFFF instead of 0xFF

#define SET_FLAGS(flags, Z, N, H, C)      \
    flags = (Z ? flag_Z : 0) |            \
            (N ? flag_N : 0) |            \
            (H ? flag_H : 0) |            \
            (C ? flag_C : 0)

#define LOG_LINE_LENGTH 68
static inline void mGBA_log_format(s_CPU* cpu, char dest[LOG_LINE_LENGTH + 1]) {
    /* format the cpu's information into a char[] like mGBA does */
    if (
        sprintf(
            dest, "A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X SP: %04X PC: 00:%04X",
            cpu->registers[r8_A],
            cpu->flags,
            cpu->registers[r8_B],
            cpu->registers[r8_C],
            cpu->registers[r8_D],
            cpu->registers[r8_E],
            cpu->registers[r8_H],
            cpu->registers[r8_L],
            cpu->SP,
            cpu->PC
        ) == -1) {

        log_fatal("Couldn't format CPU string");
    }
}

void skip_boot_rom(s_CPU* cpu);

int cpu_step(s_CPU* cpu);
void cpu_init(s_CPU* cpu);

#endif //CBOY_CPU_H
