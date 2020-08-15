int RLC_HL(s_CPU* cpu, uint8_t instruction) {
    // 0e
    log("RLC_HL %x", instruction);

    uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
    uint8_t carry = value & 0x80;
    value <<= 1;
    value |= carry ? 1 : 0;
    write_byte(cpu->mem, get_r16(cpu, r16_HL), value);
    SET_FLAGS(cpu->flags, value == 0, 0, 0, carry);
    return 16;
}

int RLC_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0 1-7
     */
    log("RLC_r8 %x", instruction);

    uint8_t carry = cpu->registers[instruction] & 0x80;
    cpu->registers[instruction] <<= 1;
    cpu->registers[instruction] |= carry ? 1 : 0;
    SET_FLAGS(cpu->flags, cpu->registers[instruction] == 0, 0, 0, carry);
    return 8;
}

int RRC_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0E
     */
    log("RRC_HL %x", instruction);
    uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
    uint8_t carry = value & 1;
    value >>= 1;
    value |= carry ? 0x80 : 0;
    write_byte(cpu->mem, get_r16(cpu, r16_HL), value);
    SET_FLAGS(cpu->flags, value == 0, 0, 0, carry);
    return 16;
}

int RRC_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0 8-F
     */
    log("RRC_r8 %x", instruction);
    uint8_t carry = cpu->registers[instruction & 0x7] & 1;
    cpu->registers[instruction & 0x7] >>= 1;
    cpu->registers[instruction & 0x7] |= carry ? 0x80 : 0;
    SET_FLAGS(cpu->flags, cpu->registers[instruction & 0x7] == 0, 0, 0, carry);
    return 8;
}

int RL_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 16
     */
    log("RL_HL %x", instruction);

    uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
    uint8_t carry = value & 0x80;
    value <<= 1;
    value |= (cpu->flags & flag_C) ? 1 : 0;
    write_byte(cpu->mem, get_r16(cpu, r16_HL), value);
    SET_FLAGS(cpu->flags, value == 0, 0, 0, carry);
    return 16;
}

int RL_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 1 0-7
     */
    log("RL_r8 %x", instruction);
    uint8_t carry;
    carry = cpu->registers[instruction & 0x7] & 0x80;
    cpu->registers[instruction & 0x7] <<= 1;
    cpu->registers[instruction & 0x7] |= (cpu->flags & flag_C) ? 1 : 0;
    SET_FLAGS(cpu->flags, cpu->registers[instruction & 0x7] == 0, 0, 0, carry);
    return 8;
}

int RR_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 1E
     */
    log("RR_HL %x", instruction);

    uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
    uint8_t carry = value & 1;
    value >>= 1;
    value |= (cpu->flags & flag_C) ? 0x80 : 0;
    write_byte(cpu->mem, get_r16(cpu, r16_HL), value);
    SET_FLAGS(cpu->flags, value == 0, 0, 0, carry);
    return 16;
}

int RR_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 1 8-F
     */
    log("RR_r8 %x", instruction);

    uint8_t carry = cpu->registers[instruction & 0x7] & 1;
    cpu->registers[instruction & 0x7] >>= 1;
    cpu->registers[instruction & 0x7] |= (cpu->flags & flag_C) ? 0x80 : 0;
    SET_FLAGS(cpu->flags, cpu->registers[instruction & 0x7] == 0, 0, 0, carry);
    return 8;
}

int RLCA(s_CPU* cpu, uint8_t instruction) {
    /*
     * 07
     */
    log("RLCA %x", instruction);
    uint8_t carry;
    // normal r8
    carry = cpu->registers[r8_A] & 0x80;
    cpu->registers[r8_A] <<= 1;
    cpu->registers[r8_A] |= carry ? 1 : 0;
    SET_FLAGS(cpu->flags, 0, 0, 0, carry);
    return 4;
}

int RRCA(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0F
     */
    log("RRCA %x", instruction);
    uint8_t carry;
    carry = cpu->registers[r8_A] & 1;
    cpu->registers[r8_A] >>= 1;
    cpu->registers[r8_A] |= carry ? 0x80 : 0;
    SET_FLAGS(cpu->flags, 0, 0, 0, carry);
    return 4;
}

int RLA(s_CPU* cpu, uint8_t instruction) {
    /*
     * 17
     */
    log("RLA %x", instruction);
    uint8_t carry;
    carry = cpu->registers[r8_A] & 0x80;
    cpu->registers[r8_A] <<= 1;
    cpu->registers[r8_A] |= (cpu->flags & flag_C) ? 1 : 0;
    SET_FLAGS(cpu->flags, 0, 0, 0, carry);
    return 4;
}

int RRA(s_CPU* cpu, uint8_t instruction) {
    /*
     * 1 8-F
     */
    log("RRA %x", instruction);
    uint8_t carry;
    carry = cpu->registers[r8_A] & 1;
    cpu->registers[r8_A] >>= 1;
    cpu->registers[r8_A] |= (cpu->flags & flag_C) ? 0x80 : 0;
    SET_FLAGS(cpu->flags, 0, 0, 0, carry);
    return 4;
}

int SLA_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 26
     */
    log("SLA_HL %x", instruction);
    uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
    uint8_t carry = value & 0x80;
    value <<= 1;
    write_byte(cpu->mem, get_r16(cpu, r16_HL), value);
    SET_FLAGS(cpu->flags, value == 0, 0, 0, carry);
    return 16;
}

int SLA_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 2 0-7
     */
    log("SLA_r8 %x", instruction);

    uint8_t carry = cpu->registers[instruction & 7] & 0x80;
    cpu->registers[instruction & 7] <<= 1;
    SET_FLAGS(cpu->flags, cpu->registers[instruction & 7] == 0, 0, 0, carry);
    return 8;
}

int SRA_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 2E
     */
    log("SRA_HL %x", instruction);
    uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
    uint8_t carry = value & 1;
    value = (uint8_t)(((int8_t)value) >> 1);
    write_byte(cpu->mem, get_r16(cpu, r16_HL), value);
    SET_FLAGS(cpu->flags, value == 0, 0, 0, carry);
    return 16;
}

int SRA_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 2 8-F
     */
    log("SRA_r8 %x", instruction);

    uint8_t carry = cpu->registers[instruction & 7] & 1;
    cpu->registers[instruction & 7] = (uint8_t)(((int8_t)cpu->registers[instruction & 7]) >> 1);
    SET_FLAGS(cpu->flags, cpu->registers[instruction & 7] == 0, 0, 0, carry);
    return 8;
}

int SRL_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 3E
     */
    log("SRL_HL %x", instruction);
    uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
    uint8_t carry = value & 1;
    value >>= 1;
    write_byte(cpu->mem, get_r16(cpu, r16_HL), value);
    SET_FLAGS(cpu->flags, value == 0, 0, 0, carry);
    return 16;
}

int SRL_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 3 8-F
     */
    log("SRL_r8 %x", instruction);

    uint8_t carry = cpu->registers[instruction & 7] & 1;
    cpu->registers[instruction & 7] >>= 1;
    SET_FLAGS(cpu->flags, cpu->registers[instruction & 7] == 0, 0, 0, carry);
    return 8;
}