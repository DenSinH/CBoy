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
