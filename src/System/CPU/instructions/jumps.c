void JP_cc(s_CPU* cpu, uint8_t instruction) {
    /*
     * 18/20/28/30/38 for different instruction codes
     */
    log("JP_cc %x", instruction);
    int8_t offset = (int8_t)read_byte(cpu->mem, cpu->PC++);

    switch(instruction) {
        case 0x18:
            // unconditional
            cpu->PC += offset;
            break;
        case 0x20:
            // NZ
            if (!(cpu->flags & flag_Z))
                cpu->PC += offset;
            break;
        case 0x28:
            // Z
            if (cpu->flags & flag_Z)
                cpu->PC += offset;
            break;
        case 0x30:
            // NC
            if (!(cpu->flags & flag_C))
                cpu->PC += offset;
            break;
        case 0x38:
            // C
            if (cpu->flags & flag_C)
                cpu->PC += offset;
            break;
        default:
            log_fatal("Invalid opcode for JP: %x", instruction);
    }
}

void CALL_cc(s_CPU* cpu, uint8_t instruction) {
    /*
     * C/D 4/C for different instruction codes
     * 110x x100
     * CD = unconditional
     */
    log("CALL_cc %x", instruction);
    uint16_t address = read_short(cpu->mem, cpu->PC);
    cpu->PC += 2;

    switch (instruction) {
        case 0xcd:
            // unconditional
            PUSH_PC(cpu);
            cpu->PC = address;
            break;
        case 0xc4:
            // NZ
            if (!(cpu->flags & flag_Z)) {
                PUSH_PC(cpu);
                cpu->PC = address;
            }
            break;
        case 0xcc:
            // Z
            if (cpu->flags & flag_Z) {
                PUSH_PC(cpu);
                cpu->PC = address;
            }
            break;
        case 0xd4:
            // NC
            if (!(cpu->flags & flag_C)) {
                PUSH_PC(cpu);
                cpu->PC = address;
            }
            break;
        case 0xdc:
            // C
            if (cpu->flags & flag_C) {
                PUSH_PC(cpu);
                cpu->PC = address;
            }
            break;
        default:
            log_fatal("unimplemented instruction: %x", instruction);
    }
}
