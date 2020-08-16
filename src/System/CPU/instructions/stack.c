void PUSH_PC(s_CPU* cpu) {
    write_byte(cpu->mem, --cpu->SP, cpu->PC >> 8);    // push HI
    write_byte(cpu->mem, --cpu->SP, cpu->PC & 0xff);  // push LO
}

void POP_PC(s_CPU* cpu) {
    cpu->PC = read_byte(cpu->mem, cpu->SP++);        // pop LO
    cpu->PC |= read_byte(cpu->mem, cpu->SP++) << 8;  // pop HI
}

int PUSH_r16(s_CPU* cpu, uint8_t instruction) {
    /*
     * c-f 5
     * 11XX 5
     */
    log("PUSH_r16 %x", instruction);

    uint8_t index = (instruction >> 4) & 3;
    if (index < 3) {
        // push normal r16
        write_byte(cpu->mem, --cpu->SP, cpu->registers[index << 1]);           // push HI
        write_byte(cpu->mem, --cpu->SP, cpu->registers[(index << 1) + 1]);     // push LO
    }
    else {
        // push AF
        write_byte(cpu->mem, --cpu->SP, cpu->registers[r8_A]);       // push HI
        write_byte(cpu->mem, --cpu->SP, cpu->flags);                 // push LO
    }
    return 16;
}

int POP_r16(s_CPU* cpu, uint8_t instruction) {
    /*
     * c-f 1
     * 11XX 1
     */
    log("POP_r16 %x", instruction);

    uint8_t index = (instruction >> 4) & 3;
    if (index < 3) {
        // pop normal r16
        cpu->registers[(index << 1) + 1] = read_byte(cpu->mem, cpu->SP++);       // pop LO
        cpu->registers[index << 1] = read_byte(cpu->mem, cpu->SP++);             // pop HI
    }
    else {
        // pop AF
        cpu->flags = read_byte(cpu->mem, cpu->SP++) & 0xf0;                      // pop LO
        cpu->registers[r8_A] = read_byte(cpu->mem, cpu->SP++);                   // pop HI
    }
    return 12;
}

int LD_u16_SP(s_CPU* cpu, uint8_t instruction) {
    // 08
    log("LD_u16_SP %x", instruction);

    uint16_t address = read_short(cpu->mem, cpu->PC);
    cpu->PC += 2;
    write_short(cpu->mem, address, cpu->SP);

    return 20;
}

int LD_SP_HL(s_CPU* cpu, uint8_t instruction) {
    // F9
    log("LD_SP_HL %x", instruction);

    cpu->SP = get_r16(cpu, r16_HL);

    return 8;
}

int ADD_SP_i8(s_CPU* cpu, uint8_t instruction) {
    // E8
    log("ADD_SP_i8 %x", instruction);

    int8_t offset = (int8_t)read_byte(cpu->mem, cpu->PC++);

    SET_FLAGS(
            cpu->flags,
            0,
            0,
            HALF_CARRY_8BIT_ADD(cpu->SP & 0xff, offset),
            ((cpu->SP & 0xff) + (uint8_t)offset) > 0xff
    );
    cpu->SP += offset;

    return 16;
}

int LD_HL_SP_i8(s_CPU* cpu, uint8_t instruction) {
    // F8
    log("LD_HL_SP %x", instruction);

    int8_t offset = (int8_t)read_byte(cpu->mem, cpu->PC++);
    set_r16(cpu, r16_HL, cpu->SP + offset);

    SET_FLAGS(
        cpu->flags,
        0,
        0,
        HALF_CARRY_8BIT_ADD(cpu->SP & 0xff, (int16_t)offset),
        ((cpu->SP & 0xff) + (uint8_t)offset) > 0xff
    );

    return 12;
}