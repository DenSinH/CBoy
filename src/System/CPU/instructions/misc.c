int NOP(s_CPU* cpu, uint8_t instruction) {
    log("NOP %x", instruction);
    return 4;
}

int DI_EI(s_CPU* cpu, uint8_t instruction) {
    /*
     * DI (F3) / EI (FB)
     */
    log("%s %x", (instruction == 0xfb) ? "EI" : "DI", instruction);
    cpu->IME = (instruction == 0xfb);
    return 4;
}

int CPL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 2F
     */
    log("CPL %x", instruction);

    cpu->registers[r8_A] = ~cpu->registers[r8_A];
    SET_FLAGS(cpu->flags, cpu->flags & flag_Z, 1, 1, cpu->flags & flag_C);
    return 4;
}

int CCF(s_CPU* cpu, uint8_t instruction) {
    /*
     * 3F
     */
    log("CCF %x", instruction);
    SET_FLAGS(cpu->flags, cpu->flags & flag_Z, 0, 0, !(cpu->flags & flag_C));
    return 4;
}

int SCF(s_CPU* cpu, uint8_t instruction) {
    /*
     * 37
     */
    log("SCF %x", instruction);
    SET_FLAGS(cpu->flags, cpu->flags & flag_Z, 0, 0, 1);
    return 4;
}

int DAA(s_CPU* cpu, uint8_t instruction) {
    /*
     * 27
     */
    log("DAA %x", instruction);
    cpu->registers[r8_A] = ((cpu->registers[r8_A] / 10) << 4) | (cpu->registers[r8_A] % 10);

    // todo: Carry flag?
    SET_FLAGS(cpu->flags, cpu->registers[r8_A] == 0, cpu->flags & flag_N, 0, cpu->flags & flag_C);
    return 4;
}