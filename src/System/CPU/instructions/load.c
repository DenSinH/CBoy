
void LD_r8_u8(s_CPU* cpu, uint8_t instruction) {
    /*
     * 0/1/2/3 6/E
     * 00RR R110
     */
    log("LD_r8_u8 %x", instruction);

    uint8_t imm = read_byte(cpu->mem, cpu->PC++);
    set_r8(cpu, instruction >> 3, imm);
}

void LD_A_r8(s_CPU* cpu, uint8_t instruction) {
    /*
     * LD r8_(instruction & 0xf) - 8, A
     */
    log("LD_A_r8 %x", instruction);
    set_r8(cpu, (instruction & 0xf) - 7, cpu->registers[r8_A]);
}

void LD_r16_u16(s_CPU* cpu, uint8_t instruction) {
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
}

void LD_r16_A(s_CPU* cpu, uint8_t instruction) {
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
}

void LD_C_A(s_CPU* cpu, uint8_t instruction) {
    /*
     * E0 / E2 / F0 / F2
     */
    log("LD_C_A %x", instruction);
    switch (instruction) {
        case 0xe0:
            // LD (FF00 + u8), A
            write_byte(cpu->mem, 0xff00 + read_byte(cpu->mem, cpu->PC++), cpu->registers[r8_A]);
            break;
        case 0xe2:
            // LD (FF00 + C), A
            write_byte(cpu->mem, 0xff00 + ((cpu->flags & flag_C) ? 1 : 0), cpu->registers[r8_A]);
            break;
        case 0xf0:
            // LD A, (FF00 + u8)
            cpu->registers[r8_A] = read_byte(cpu->mem, 0xff00 + read_byte(cpu->mem, cpu->PC++));
            break;
        case 0xf2:
            // LD A, (FF00 + C)
            cpu->registers[r8_A] = read_byte(cpu->mem, 0xff00 + ((cpu->flags & flag_C) ? 1 : 0));
            break;
        default:
            log_fatal("Invalid LD_C_A instruction: %x", instruction);
    }
}