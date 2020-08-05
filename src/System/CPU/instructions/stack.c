void PUSH_PC(s_CPU* cpu) {
    write_byte(cpu->mem, --cpu->SP, cpu->PC >> 8);    // push HI
    write_byte(cpu->mem, --cpu->SP, cpu->PC & 0xff);  // push LO
}

void PUSH_r16(s_CPU* cpu, uint8_t instruction) {
    /*
     * c-f 5
     * 11XX 5
     */
    log("PUSH_r16 %x", instruction);

    uint8_t index = (instruction >> 4) & 2;
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
}

void POP_r16(s_CPU* cpu, uint8_t instruction) {
    /*
     * c-f 1
     * 11XX 1
     */
    log("POP_r16 %x", instruction);

    uint8_t index = (instruction >> 4) & 2;
    if (index < 3) {
        // pop normal r16
        cpu->registers[(index << 1) + 1] = read_byte(cpu->mem, cpu->SP++);       // pop LO
        cpu->registers[index << 1] = read_byte(cpu->mem, cpu->SP++);             // pop HI
    }
    else {
        // pop AF
        cpu->registers[r8_A] = read_byte(cpu->mem, cpu->SP++);       // pop LO
        cpu->flags = read_byte(cpu->mem, cpu->SP++);                 // pop HI
    }
}