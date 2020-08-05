void XOR_A_r8(s_CPU* cpu, uint8_t instruction) {
    /* A8 - AF: XOR with register LSNibble - 8 */
    log("XOR_A_r8 %x", instruction);
    cpu->registers[r8_A] ^= get_r8(cpu, (instruction & 0xf) - 8);
    SET_FLAGS(cpu->flags, cpu->registers[r8_A] == 0, 0, 0, 0)
}

void INC_r8(struct s_CPU* cpu, uint8_t instruction) {
    /*
     * 0/1/2/3 4/C
     * 00RR R100
     */
    log("INC_r8 %x", instruction);
    uint8_t old_val;

    uint8_t r8 = instruction >> 3;
    if (r8 != 7) {
        old_val = cpu->registers[r8]++;
        SET_FLAGS(cpu->flags, cpu->registers[r8] == 0, 0, HALF_CARRY(old_val, cpu->registers[r8]), 0);
    }
    else {
        set_r8(cpu, 7, (old_val = get_r8(cpu, 7)) + 1);
        SET_FLAGS(cpu->flags, cpu->registers[r8] == 0, 0, HALF_CARRY(old_val, old_val - 1), 0);
    }

}