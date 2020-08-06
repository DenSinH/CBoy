void JP_cc_offset(s_CPU* cpu, uint8_t instruction) {
    /*
     * 18/20/28/30/38 for different instruction codes
     */
    log("JP_cc_offset %x", instruction);
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
     * C/D 4/C for different condition codes
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

void RET_cc(s_CPU* cpu, uint8_t instruction) {
    /*
     * C/D 0/8/9 for different condition codes
     * 110x x00x
     */
    log("RET_cc %x", instruction);

    switch (instruction) {
        case 0xc0:
            // NZ
            if (!(cpu->flags & flag_Z)) {
                POP_PC(cpu);
            }
            break;
        case 0xd0:
            // NC
            if (!(cpu->flags & flag_C)) {
                POP_PC(cpu);
            }
            break;
        case 0xc8:
            // Z
            if (cpu->flags & flag_Z) {
                POP_PC(cpu);
            }
            break;
        case 0xd8:
            // C
            if (cpu->flags & flag_C) {
                POP_PC(cpu);
            }
            break;
        case 0xc9:
            // unconditional
            POP_PC(cpu);
            break;
        case 0xd9:
            // RETI
            log_fatal("Unimplemented instruction: RETI");
            break;
        default:
            log_fatal("unimplemented instruction: %x", instruction);
    }
}
