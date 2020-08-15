int BIT_u3_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 4/5/6/7 0-F
     * 01BB BRRR: if (registers[RRR] bit BBB is set: set Z
     */

    log("BIT_u3_r8 %x", instruction);

    SET_FLAGS(
            cpu->flags,
            !(get_r8(cpu, instruction & 7) & (1 << ((instruction & 0x38) >> 3))),
            0,
            1,
            cpu->flags & flag_C);

    return 4;
}
