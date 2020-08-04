#include "../CPU.h"
#include "../../MEM/MEM.h"

void LD_u16(s_CPU* cpu, uint8_t instruction) {
    /*
     * LD BC, u16  : 0x01
     * LD DE, u16  : 0x11
     * LD HL, u16  : 0x21
     * LD SP, u16  : 0x31
     *
     * so we can use instruction >> 3 for the r16 index
     */
    uint16_t value = read_short(cpu->mem, cpu->PC);
    cpu->PC += 2;

    set_r16(cpu, instruction >> 3, value);
}