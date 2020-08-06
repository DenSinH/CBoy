void NOP(s_CPU* cpu, uint8_t instruction) {
    log("NOP %x", instruction);
}

void DI_EI(s_CPU* cpu, uint8_t instruction) {
    /*
     * DI (F3) / EI (FB)
     */
    log("%s %x", (instruction == 0xfb) ? "EI" : "DI", instruction);
    cpu->IME = (instruction == 0xfb);
}
