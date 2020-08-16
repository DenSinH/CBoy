#include "../CPU.h"

int BIT_u3_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 4/5/6/7 0-F
     * 01BB B101: if (registers[RRR] bit BBB is set: set Z
     */

    log("BIT_u3_HL %x", instruction);

    SET_FLAGS(
            cpu->flags,
            !(read_byte(cpu->mem, get_r16(cpu, r16_HL)) & (1 << ((instruction & 0x38) >> 3))),
            0,
            1,
            cpu->flags & flag_C);

    return 4;
}


int BIT_u3_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 4/5/6/7 0-F
     * 01BB BRRR: if (registers[RRR] bit BBB is set: set Z
     */

    log("BIT_u3_r8 %x", instruction);

    SET_FLAGS(
            cpu->flags,
            !(cpu->registers[instruction & 0x7] & (1 << ((instruction & 0x38) >> 3))),
            0,
            1,
            cpu->flags & flag_C);

    return 4;
}

int SWAP_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 36
     */
    log("SWAP_HL %x", instruction);

    uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
    value = (value << 4) | (value >> 4);
    SET_FLAGS(cpu->flags, value == 0, 0, 0, 0);

    return 16;
}

int SWAP_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 3 0-7
     * 0011 0RRR
     */
    log("SWAP_r8 %x", instruction);

    cpu->registers[instruction & 7] = (cpu->registers[instruction & 7] << 4) | (cpu->registers[instruction & 7] >> 4);
    SET_FLAGS(cpu->flags, cpu->registers[instruction & 7] == 0, 0, 0, 0);

    return 8;
}

int RES_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 8-B 6/E
     * 10BB B110
     */
    log("RES_HL %x", instruction)

    uint8_t bit = (instruction >> 3) & 0x07;
    uint16_t HL = get_r16(cpu, r16_HL);
    write_byte(cpu->mem, HL, read_byte(cpu->mem, HL) & (~(1 << bit)));
    return 16;
}

int RES_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 8-B XXXX
     * 10BB BRRR
     */
    log("RES_r8 %x", instruction)

    uint8_t bit = (instruction >> 3) & 0x07;
    cpu->registers[instruction & 0x07] &= ~(1 << bit);
    return 8;
}

int SET_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * C-F 6/E
     * 11BB B110
     */
    log("SET_HL %x", instruction)

    uint8_t bit = (instruction >> 3) & 0x07;
    uint16_t HL = get_r16(cpu, r16_HL);
    write_byte(cpu->mem, HL, read_byte(cpu->mem, HL) | (1 << bit));
    return 16;
}

int SET_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * C-F 6/E
     * 11BB BRRR
     */
    log("SET_r8 %x", instruction)

    uint8_t bit = (instruction >> 3) & 0x07;
    cpu->registers[instruction & 0x07] |= 1 << bit;
    return 8;
}