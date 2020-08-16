
int LD_r8_u8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0/1/2/3 6/E
     * 00RR R110
     */
    log("LD_r8_u8 %x", instruction);

    uint8_t value = read_byte(cpu->mem, cpu->PC++);
    set_r8(cpu, instruction >> 3, value);
    return ((instruction >> 3) == r8_HL) ? 12 : 8;
}

int LD_HL_r8_HALT(s_CPU* cpu, uint8_t instruction) {
    /*
     * 7 0-7
     * 0111 0RRR
     */
    if ((instruction & 0x07) != 6) {
        log("LD_HL_r8 %x", instruction);
        write_byte(cpu->mem, get_r16(cpu, r16_HL), cpu->registers[instruction & 0x07]);
        return 8;
    }
    else {
        // log_fatal("unimplemented instruction: HALT");
        return 8;
    }
}

int LD_r8_HL(s_CPU* cpu, uint8_t instruction) {
    /*
     * 4-7 6/e
     * 01RR R110
     */
    log("LD_r8_HL %x", instruction);
    cpu->registers[(instruction >> 3) & 7] = read_byte(cpu->mem, get_r16(cpu, r16_HL));
    return 8;
}

int LD_r8_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 4 - 7 0 - F
     * 01XX XYYY: LD XXX, YYY
     * We already handle the (HL) cases above
     */
    log("LD_r8_r8 %x", instruction);
    cpu->registers[(instruction >> 3) & 7] = cpu->registers[instruction & 7];
    return 4;
}

int LD_r16_u16(s_CPU* cpu, uint8_t instruction) {
    /*
     * LD BC, u16  : 0x01
     * LD DE, u16  : 0x11
     * LD HL, u16  : 0x21
     * LD SP, u16  : 0x31
     *
     * so we can use instruction >> 3 for the r16 index
     */
    log("LD_r16_u16 %x", instruction);
    uint16_t value = read_short(cpu->mem, cpu->PC);
    cpu->PC += 2;

    set_r16(cpu, instruction >> 3, value);
    return 12;
}

int LD_r16_A(s_CPU* cpu, uint8_t instruction) {
    /*
     * LD [BC], A   : 0x02
     * LD [DE], A   : 0x12
     * LD [HL+], A  : 0x22
     * LD [HL-], A  : 0x32
     */
    log("LD_r16_A %x", instruction);
    switch (instruction >> 4) {
        case 0:
            write_byte(cpu->mem, get_r16(cpu, r16_BC), cpu->registers[r8_A]);
            break;
        case 1:
            write_byte(cpu->mem, get_r16(cpu, r16_DE), cpu->registers[r8_A]);
            break;
        case 2:
            write_byte(cpu->mem, get_r16(cpu, r16_HL), cpu->registers[r8_A]);
            set_r16(cpu, r16_HL, get_r16(cpu, r16_HL) + 1);
            break;
        case 3:
            write_byte(cpu->mem, get_r16(cpu, r16_HL), cpu->registers[r8_A]);
            set_r16(cpu, r16_HL, get_r16(cpu, r16_HL) - 1);
            break;
    }
    return 8;
}

int LD_A_r16(s_CPU* cpu, uint8_t instruction) {
    /*
     * LD A, [BC]   : 0x0a
     * LD A, [DE]   : 0x1a
     * LD A, [HL+]  : 0x2a
     * LD A, [HL-]  : 0x3a
     */
    log("LD_A_r16 %x", instruction);
    switch (instruction >> 4) {
        case 0:
            cpu->registers[r8_A] = read_byte(cpu->mem, get_r16(cpu, r16_BC));
            break;
        case 1:
            cpu->registers[r8_A] = read_byte(cpu->mem, get_r16(cpu, r16_DE));
            break;
        case 2:
            cpu->registers[r8_A] = read_byte(cpu->mem, get_r16(cpu, r16_HL));
            set_r16(cpu, r16_HL, get_r16(cpu, r16_HL) + 1);
            break;
        case 3:
            cpu->registers[r8_A] = read_byte(cpu->mem, get_r16(cpu, r16_HL));
            set_r16(cpu, r16_HL, get_r16(cpu, r16_HL) - 1);
            break;
    }
    return 8;
}

int LD_FF00_A(s_CPU* cpu, uint8_t instruction) {
    /*
     * E0 / E2 / F0 / F2
     */
    log("LD_FF00_A %x", instruction);
    switch (instruction) {
        case 0xe0:
            // LD (FF00 + u8), A
            write_byte(cpu->mem, 0xff00 + read_byte(cpu->mem, cpu->PC++), cpu->registers[r8_A]);
            return 12;
        case 0xe2:
            // LD (FF00 + C), A
            write_byte(cpu->mem, 0xff00 + cpu->registers[r8_C], cpu->registers[r8_A]);
            return 8;
        case 0xf0:
            // LD A, (FF00 + u8)
            cpu->registers[r8_A] = read_byte(cpu->mem, 0xff00 + read_byte(cpu->mem, cpu->PC++));
            return 12;
        case 0xf2:
            // LD A, (FF00 + C)
            cpu->registers[r8_A] = read_byte(cpu->mem, 0xff00 + cpu->registers[r8_C]);
            return 8;
        default:
            log_fatal("Invalid LD_FF00_A instruction: %x", instruction);
    }
}

int LD_u16_A(s_CPU* cpu, uint8_t instruction) {
    /*
     * E/F A
     */
    log("LD_u16_A %x", instruction);
    uint16_t address = read_short(cpu->mem, cpu->PC);
    cpu->PC += 2;

    if (instruction == 0xEA) {
        // LD (u16), A
        write_byte(cpu->mem, address, cpu->registers[r8_A]);
    }
    else {
        // LD A, (u16)
        cpu->registers[r8_A] = read_byte(cpu->mem, address);
    }
    return 16;
}