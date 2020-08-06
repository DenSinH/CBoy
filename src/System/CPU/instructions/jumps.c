int JP_cc_offset(s_CPU* cpu, uint8_t instruction) {
    /*
     * 18/20/28/30/38 for different instruction codes
     */
    log("JP_cc_offset %x", instruction);
    int8_t offset = (int8_t)read_byte(cpu->mem, cpu->PC++);

    switch(instruction) {
        case 0x18:
            // unconditional
            cpu->PC += offset;
            return 12;
        case 0x20:
            // NZ
            if (!(cpu->flags & flag_Z)) {
                cpu->PC += offset;
                return 12;
            }
            return 8;
        case 0x28:
            // Z
            if (cpu->flags & flag_Z) {
                cpu->PC += offset;
                return 12;
            }
            return 8;
        case 0x30:
            // NC
            if (!(cpu->flags & flag_C)) {
                cpu->PC += offset;
                return 12;
            }
            return 8;
        case 0x38:
            // C
            if (cpu->flags & flag_C) {
                cpu->PC += offset;
                return 12;
            }
            return 8;
        default:
            log_fatal("Invalid opcode for JP_cc offset: %x", instruction);
    }
}

int JP_cc_direct_unused(s_CPU* cpu, uint8_t instruction) {
    /*
     * C/D 2/3/A for different instruction codes
     * 110X X01Y
     * Y should actually be 0, but the cases that are excessively thrown in here are:
     * C3: unused: crash
     * CB: prefix, never happens
     * D3: unused: crash
     * DB: unused: crash
     */
    log("JP_cc_direct %x", instruction);
    uint16_t address = read_short(cpu->mem, cpu->PC);
    cpu->PC += 2;

    switch(instruction) {
        case 0xc2:
            // NZ
            if (!(cpu->flags & flag_Z)) {
                cpu->PC = address;
                return 16;
            }
            return 12;
        case 0xd2:
            // NC
            if (!(cpu->flags & flag_C)) {
                cpu->PC = address;
                return 16;
            }
            return 12;
        case 0xc3:
            // unconditional
            cpu->PC = address;
            return 16;
        case 0xca:
            // Z
            if (cpu->flags & flag_Z) {
                cpu->PC = address;
                return 16;
            }
            return 12;
        case 0xda:
            // C
            if (cpu->flags & flag_C) {
                cpu->PC = address;
                return 16;
            }
            return 12;
        default:
            log_fatal("Invalid opcode for JP_cc direct: %x", instruction);
    }
}

int CALL_cc(s_CPU* cpu, uint8_t instruction) {
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
            return 24;
        case 0xc4:
            // NZ
            if (!(cpu->flags & flag_Z)) {
                PUSH_PC(cpu);
                cpu->PC = address;
                return 24;
            }
            return 12;
        case 0xcc:
            // Z
            if (cpu->flags & flag_Z) {
                PUSH_PC(cpu);
                cpu->PC = address;
                return 24;
            }
            return 12;
        case 0xd4:
            // NC
            if (!(cpu->flags & flag_C)) {
                PUSH_PC(cpu);
                cpu->PC = address;
                return 24;
            }
            return 12;
        case 0xdc:
            // C
            if (cpu->flags & flag_C) {
                PUSH_PC(cpu);
                cpu->PC = address;
                return 24;
            }
            return 12;
        default:
            log_fatal("unimplemented instruction: %x", instruction);
    }
}

int RET_cc(s_CPU* cpu, uint8_t instruction) {
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
                return 20;
            }
            return 8;
        case 0xd0:
            // NC
            if (!(cpu->flags & flag_C)) {
                POP_PC(cpu);
                return 20;
            }
            return 8;
        case 0xc8:
            // Z
            if (cpu->flags & flag_Z) {
                POP_PC(cpu);
                return 20;
            }
            return 8;
        case 0xd8:
            // C
            if (cpu->flags & flag_C) {
                POP_PC(cpu);
                return 20;
            }
            return 8;
        case 0xc9:
            // unconditional
            POP_PC(cpu);
            return 20;
            break;
        case 0xd9:
            // RETI
            log_fatal("Unimplemented instruction: RETI");
            break;
        default:
            log_fatal("unimplemented instruction: %x", instruction);
    }
}
