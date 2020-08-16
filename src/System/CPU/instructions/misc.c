int NOP(s_CPU* cpu, uint8_t instruction) {
    log("NOP %x", instruction);
    return 4;
}

int DI_EI(s_CPU* cpu, uint8_t instruction) {
    /*
     * DI (F3) / EI (FB)
     */
    log("%s %x", (instruction == 0xfb) ? "EI" : "DI", instruction);
    cpu->IO->IME = (instruction == 0xfb);
    return 4;
}

int CPL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 2F
     */
    log("CPL %x", instruction);

    cpu->registers[r8_A] = ~cpu->registers[r8_A];
    cpu->flags.N =  1;
    cpu->flags.H =  1;
    return 4;
}

int CCF(s_CPU* cpu, uint8_t instruction) {
    /*
     * 3F
     */
    log("CCF %x", instruction);
    cpu->flags.N =  0;
    cpu->flags.H =  0;
    cpu->flags.C =  !cpu->flags.C;
    return 4;
}

int SCF(s_CPU* cpu, uint8_t instruction) {
    /*
     * 37
     */
    log("SCF %x", instruction);
    cpu->flags.N =  0;
    cpu->flags.H =  0;
    cpu->flags.C =  1;
    return 4;
}

int DAA(s_CPU* cpu, uint8_t instruction) {
    /*
     * 27
     */
    log("DAA %x", instruction);
    /* thank you https://ehaskins.com/2018-01-30%20Z80%20DAA/ */
    uint8_t correction = 0;
    bool setC = false;

    if ((cpu->flags.H) || (!(cpu->flags.N) && ((cpu->registers[r8_A] & 0xf) > 9))) {
        correction |= 6;
    }

    if ((cpu->flags.C) || ((!(cpu->flags.N) && (cpu->registers[r8_A] > 0x99)))) {
        correction |= 0x60;
        setC = true;
    }

    cpu->registers[r8_A] += (cpu->flags.N) ? -correction : correction;

    cpu->flags.Z = cpu->registers[r8_A] == 0;
    cpu->flags.N = cpu->flags.N;
    cpu->flags.H = 0;
    cpu->flags.C = setC;
    return 4;
}