void RLC_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0 1-7
     */
    log("RLC_r8 %x", instruction);
    uint8_t carry;
    if (instruction == 0x6) {
        // HL
        uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
        carry = value & 0x80;
        value <<= 1;
        value |= carry ? 1 : 0;
        write_byte(cpu->mem, get_r16(cpu, r16_HL), value);
        SET_FLAGS(cpu->flags, value == 0, 0, 0, carry);
    }
    else {
        // normal r8
        carry = cpu->registers[instruction] & 0x80;
        cpu->registers[instruction] <<= 1;
        cpu->registers[instruction] |= carry ? 1 : 0;
        SET_FLAGS(cpu->flags, cpu->registers[instruction] == 0, 0, 0, carry);
    }
}

void RRC_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0 8-F
     */
    log("RRC_r8 %x", instruction);
    uint8_t carry;
    if (instruction == 0xe) {
        // HL
        uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
        carry = value & 1;
        value >>= 1;
        value |= carry ? 0x80 : 0;
        write_byte(cpu->mem, get_r16(cpu, r16_HL), value);
        SET_FLAGS(cpu->flags, value == 0, 0, 0, carry);
    }
    else {
        // normal r8
        carry = cpu->registers[instruction & 0x7] & 1;
        cpu->registers[instruction & 0x7] >>= 1;
        cpu->registers[instruction & 0x7] |= carry ? 0x80 : 0;
        SET_FLAGS(cpu->flags, cpu->registers[instruction & 0x7] == 0, 0, 0, carry);
    }
}

void RL_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 1 0-7
     */
    log("RL_r8 %x", instruction);
    uint8_t carry;
    if (instruction == 0x16) {
        // HL
        uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
        carry = value & 0x80;
        value <<= 1;
        value |= (cpu->flags & flag_C) ? 1 : 0;
        write_byte(cpu->mem, get_r16(cpu, r16_HL), value);
        SET_FLAGS(cpu->flags, value == 0, 0, 0, carry);
    }
    else {
        // normal r8
        carry = cpu->registers[instruction & 0x7] & 0x80;
        cpu->registers[instruction & 0x7] <<= 1;
        cpu->registers[instruction & 0x7] |= (cpu->flags & flag_C) ? 1 : 0;
        SET_FLAGS(cpu->flags, cpu->registers[instruction & 0x7] == 0, 0, 0, carry);
    }
}

void RR_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 1 8-F
     */
    log("RR_r8 %x", instruction);
    uint8_t carry;
    if (instruction == 0x1e) {
        // HL
        uint8_t value = read_byte(cpu->mem, get_r16(cpu, r16_HL));
        carry = value & 1;
        value >>= 1;
        value |= (cpu->flags & flag_C) ? 0x80 : 0;
        write_byte(cpu->mem, get_r16(cpu, r16_HL), value);
        SET_FLAGS(cpu->flags, value == 0, 0, 0, carry);
    }
    else {
        // normal r8
        carry = cpu->registers[instruction & 0x7] & 1;
        cpu->registers[instruction & 0x7] >>= 1;
        cpu->registers[instruction & 0x7] |= (cpu->flags & flag_C) ? 0x80 : 0;
        SET_FLAGS(cpu->flags, cpu->registers[instruction & 0x7] == 0, 0, 0, carry);
    }
}

void RLCA(s_CPU* cpu, uint8_t instruction) {
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
}

void RRCA(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0F
     */
    log("RRCA %x", instruction);
    uint8_t carry;
    carry = cpu->registers[r8_A] & 1;
    cpu->registers[r8_A] >>= 1;
    cpu->registers[r8_A] |= carry ? 0x80 : 0;
    SET_FLAGS(cpu->flags, 0, 0, 0, carry);
}

void RLA(s_CPU* cpu, uint8_t instruction) {
    /*
     * 17
     */
    log("RLA %x", instruction);
    uint8_t carry;
    carry = cpu->registers[r8_A] & 0x80;
    cpu->registers[r8_A] <<= 1;
    cpu->registers[r8_A] |= (cpu->flags & flag_C) ? 1 : 0;
    SET_FLAGS(cpu->flags, 0, 0, 0, carry);
}

void RRA(s_CPU* cpu, uint8_t instruction) {
    /*
     * 1 8-F
     */
    log("RRA %x", instruction);
    uint8_t carry;
    carry = cpu->registers[r8_A] & 1;
    cpu->registers[r8_A] >>= 1;
    cpu->registers[r8_A] |= (cpu->flags & flag_C) ? 0x80 : 0;
    SET_FLAGS(cpu->flags, 0, 0, 0, carry);
}