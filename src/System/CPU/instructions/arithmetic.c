int INC_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0/1/2/3 4/C
     * 00RR R100
     */
    log("INC_r8 %x", instruction);
    uint8_t old_val;

    uint8_t r8 = instruction >> 3;
    if (r8 != r8_HL) {
        old_val = cpu->registers[r8]++;
        cpu->flags.Z =  cpu->registers[r8] == 0;
        cpu->flags.N =  0;
        cpu->flags.H =  HALF_CARRY_8BIT_ADD(old_val, 1);
        return 4;
    }
    else {
        old_val = read_byte(cpu->mem, get_r16(cpu, r16_HL));
        write_byte(cpu->mem, get_r16(cpu, r16_HL), old_val + 1);
        cpu->flags.Z =  (uint8_t)(old_val + 1) == 0;
        cpu->flags.N =  0;
        cpu->flags.H =  HALF_CARRY_8BIT_ADD(old_val, 1);
        return 12;
    }
}

int INC_r16(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0/1/2/3 3
     * 00RR 0011
     */
    log("INC_r16 %x", instruction);

    uint8_t r16 = instruction >> 3;  // use the fact that bit 3 is 0
    set_r16(cpu, r16, get_r16(cpu, r16) + 1);
    return 8;
}

int DEC_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0/1/2/3 5/D
     * 00RR R101
     */
    log("DEC_r8 %x", instruction);
    uint8_t old_val;

    uint8_t r8 = instruction >> 3;
    if (r8 != r8_HL) {
        old_val = cpu->registers[r8]--;
        cpu->flags.Z =  cpu->registers[r8] == 0;
        cpu->flags.N =  1;
        cpu->flags.H =  HALF_CARRY_8BIT_SUB(old_val, 1);
        return 4;
    }
    else {
        uint16_t HL = get_r16(cpu, r16_HL);
        old_val = read_byte(cpu->mem, HL);
        write_byte(cpu->mem, HL, old_val - 1);

        cpu->flags.Z =  (old_val - 1) == 0;
        cpu->flags.N =  1;
        cpu->flags.H =  HALF_CARRY_8BIT_SUB(old_val, 1);
        return 12;
    }
}

int DEC_r16(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0-3 B
     * 00RR 1011
     */
    log("INC_r16 %x", instruction);

    uint8_t r16 = (instruction >> 3) & 0x6;
    set_r16(cpu, r16, get_r16(cpu, r16) - 1);
    return 8;
}

int ADD_HL_r16(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0-3 9
     * 00RR 1001
     */
    log("ADD_r16 %x", instruction);
    uint16_t old_val, operand;
    old_val = get_r16(cpu, r16_HL);
    operand = get_r16(cpu, (instruction >> 3) & 0xfffe);
    set_r16(cpu, r16_HL, old_val + operand);

    cpu->flags.N = 0;
    cpu->flags.H = HALF_CARRY_16BIT_ADD(old_val, operand);
    cpu->flags.C = old_val + operand > 0xffff;

    return 8;
}


static inline void ARITH_A(s_CPU* cpu, uint8_t opcode, uint8_t operand) {
    log("Operating on A: code %x with %02x", opcode, operand);
    uint8_t old_val = cpu->registers[r8_A];
    switch (opcode) {
        case 0x0:
            // ADD
            cpu->registers[r8_A] += operand;
            cpu->flags.Z = cpu->registers[r8_A] == 0;
            cpu->flags.N = 0;
            cpu->flags.H = HALF_CARRY_8BIT_ADD(old_val, operand);
            cpu->flags.C = (old_val + operand) > 0xff;
            break;
        case 0x1:
            // ADC
            cpu->registers[r8_A] += operand + (cpu->flags.C ? 1 : 0);

            cpu->flags.Z = cpu->registers[r8_A] == 0;
            cpu->flags.N = 0;
            cpu->flags.H = HALF_CARRY_8BIT_ADD_C(old_val, operand, cpu->flags.C ? 1 : 0);
            cpu->flags.C = (old_val + operand + (cpu->flags.C ? 1 : 0)) > 0xff;
            break;
        case 0x2:
            // SUB
            cpu->registers[r8_A] -= operand;
            cpu->flags.Z = cpu->registers[r8_A] == 0;
            cpu->flags.N = 1;
            cpu->flags.H = HALF_CARRY_8BIT_SUB(old_val, operand);
            // I looked up how uint8_t's behaved for this:
            cpu->flags.C = (old_val - operand) < 0;
            break;
        case 0x3:
            // SBC
            cpu->registers[r8_A] -= operand + (cpu->flags.C ? 1 : 0);
            cpu->flags.Z = cpu->registers[r8_A] == 0;
            cpu->flags.N = 1;
            cpu->flags.H = HALF_CARRY_8BIT_SUB_C(old_val, operand, cpu->flags.C ? 1 : 0);
            cpu->flags.C = (old_val - (operand + (cpu->flags.C ? 1 : 0))) < 0;
            break;
        case 0x4:
            // AND
            cpu->registers[r8_A] &= operand;
            cpu->flags.Z = cpu->registers[r8_A] == 0;
            cpu->flags.N = 0;
            cpu->flags.H = 1;
            cpu->flags.C = 0;
            break;
        case 0x5:
            // XOR
            cpu->registers[r8_A] ^= operand;
            cpu->flags.Z = cpu->registers[r8_A] == 0;
            cpu->flags.N = 0;
            cpu->flags.H = 0;
            cpu->flags.C = 0;
            break;
        case 0x6:
            // OR
            cpu->registers[r8_A] |= operand;
            cpu->flags.Z = cpu->registers[r8_A] == 0;
            cpu->flags.N = 0;
            cpu->flags.H = 0;
            cpu->flags.C = 0;
            break;
        case 0x7:
            // CP
            // SUB without storing

            cpu->flags.Z = (cpu->registers[r8_A] - operand) == 0;
            cpu->flags.N = 1;
            cpu->flags.H = HALF_CARRY_8BIT_SUB(cpu->registers[r8_A], operand);
            // I looked up how uint8_t's behaved for this:
            cpu->flags.C = (cpu->registers[r8_A] - operand) < 0;
            break;
        default:
            log_fatal("invalid opcode for arithmetic A: %x", opcode);
    }
}

int ARITH_A_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 8-B 6/E
     * 10XX X110
     * opcode XXX
     */
    log("ARITH_A_HL %x", instruction);
    uint8_t operand = read_byte(cpu->mem, get_r16(cpu, r16_HL));
    ARITH_A(cpu, (instruction >> 3) & 0x7, operand);
    return 8;
}

int ARITH_A_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 8-B ~(6/E)
     * 10XX XRRR
     * opcode XXX, register RRR
     */
    log("ARITH_A_r8 %x", instruction);
    ARITH_A(cpu, (instruction >> 3) & 0x7, cpu->registers[instruction & 7]);
    return 4;
}

int ARITH_A_u8(s_CPU* cpu, uint8_t instruction) {
    /*
     * C-F 6/E
     * 11XX X110
     * opcode XXX
     */
    log("ARITH_A_u8 %x", instruction);
    uint8_t operand = read_byte(cpu->mem, cpu->PC++);
    ARITH_A(cpu, (instruction >> 3) & 0x7, operand);
    return 8;
}