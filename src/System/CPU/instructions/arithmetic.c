void INC_r8(s_CPU* cpu, uint8_t instruction) {
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
        SET_FLAGS(cpu->flags, old_val + 1, 0, HALF_CARRY(old_val, old_val + 1), cpu->flags & flag_C);
    }
}

void INC_r16(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0/1/2/3 3
     * 00RR 0011
     */
    log("INC_r16 %x", instruction);

    uint8_t r16 = instruction >> 3;  // use the fact that bit 3 is 0
    set_r16(cpu, r16, get_r16(cpu, r16) + 1);
}

void DEC_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0/1/2/3 5/D
     * 00RR R101
     */
    log("DEC_r8 %x", instruction);
    uint8_t old_val;

    uint8_t r8 = instruction >> 3;
    if (r8 != 7) {
        old_val = cpu->registers[r8]--;
        SET_FLAGS(cpu->flags, cpu->registers[r8] == 0, 1, HALF_CARRY(old_val, cpu->registers[r8]), cpu->flags & flag_C);
    }
    else {
        set_r8(cpu, 7, (old_val = get_r8(cpu, 7)) - 1);
        SET_FLAGS(cpu->flags, old_val - 1, 1, HALF_CARRY(old_val, old_val - 1), cpu->flags & flag_C);
    }
}

void DEC_r16(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0-3 B
     * 00RR 1011
     */
    log("INC_r16 %x", instruction);

    uint8_t r16 = (instruction >> 3) & 0x6;
    set_r16(cpu, r16, get_r16(cpu, r16) - 1);
}


void ARITH_A(s_CPU* cpu, uint8_t opcode, uint8_t operand) {
    uint8_t old_val = cpu->registers[r8_A];
    switch (opcode) {
        case 0x0:
            // ADD
            cpu->registers[r8_A] += operand;
            SET_FLAGS(
                    cpu->flags,
                    cpu->registers[r8_A] == 0,
                    0,
                    HALF_CARRY(old_val, cpu->registers[r8_A]),
                    (old_val + operand) > 0xff
            );
            break;
        case 0x1:
            // ADC
            cpu->registers[r8_A] += operand + ((cpu->flags & flag_C) ? 1 : 0);
            SET_FLAGS(
                    cpu->flags,
                    cpu->registers[r8_A] == 0,
                    0,
                    HALF_CARRY(old_val, cpu->registers[r8_A]),
                    (old_val + ((cpu->flags & flag_C) ? 1 : 0)) > 0xff
            );
            break;
        case 0x2:
            // SUB
            cpu->registers[r8_A] -= operand;
            SET_FLAGS(
                    cpu->flags,
                    cpu->registers[r8_A] == 0,
                    1,
                    HALF_CARRY(old_val, cpu->registers[r8_A]),
                    // I looked up how uint8_t's behaved for this:
                    (old_val - operand) < 0
            );
            break;
        case 0x3:
            // SBC
            cpu->registers[r8_A] -= operand + ((cpu->flags & flag_C) ? 1 : 0);
            SET_FLAGS(
                    cpu->flags,
                    cpu->registers[r8_A] == 0,
                    1,
                    HALF_CARRY(old_val, cpu->registers[r8_A]),
            // I looked up how uint8_t's behaved for this:
                    (old_val - ((cpu->flags & flag_C) ? 1 : 0)) < 0
            );
            break;
        case 0x4:
            // AND
            cpu->registers[r8_A] &= operand;
            SET_FLAGS(
                    cpu->flags,
                    cpu->registers[r8_A] == 0,
                    0,
                    1,
                    0
            );
            break;
        case 0x5:
            // XOR
            cpu->registers[r8_A] ^= operand;
            SET_FLAGS(
                    cpu->flags,
                    cpu->registers[r8_A] == 0,
                    0,
                    0,
                    0
            );
            break;
        case 0x6:
            // OR
            cpu->registers[r8_A] |= operand;
            SET_FLAGS(
                    cpu->flags,
                    cpu->registers[r8_A] == 0,
                    0,
                    0,
                    0
            );
            break;
        case 0x7:
            // CP
            // SUB without storing, so we copy the old value back
            cpu->registers[r8_A] -= operand;
            SET_FLAGS(
                    cpu->flags,
                    cpu->registers[r8_A] == 0,
                    1,
                    HALF_CARRY(old_val, cpu->registers[r8_A]),
                    // I looked up how uint8_t's behaved for this:
                    (old_val - operand) < 0
            );

            cpu->registers[r8_A] = old_val;
            break;
        default:
            log_fatal("invalid opcode for arithmetic A: %x", opcode);
    }
}

void ARITH_A_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 8-B 6/E
     * 10XX X110
     * opcode XXX
     */
    log("ARITH_A_HL %x", instruction);
    uint8_t operand = read_byte(cpu->mem, get_r16(cpu, r16_HL));
    ARITH_A(cpu, (instruction >> 3) & 0x7, operand);
}

void ARITH_A_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 8-B ~(6/E)
     * 10XX XRRR
     * opcode XXX, register RRR
     */
    log("ARITH_A_r8 %x", instruction);
    ARITH_A(cpu, (instruction >> 3) & 0x7, cpu->registers[instruction & 7]);
}

void ARITH_A_u8(s_CPU* cpu, uint8_t instruction) {
    /*
     * C-F 6/E
     * 11XX X110
     * opcode XXX
     */
    log("ARITH_A_u8 %x", instruction);
    uint8_t operand = read_byte(cpu->mem, cpu->PC++);
    ARITH_A(cpu, (instruction >> 3) & 0x7, operand);
}