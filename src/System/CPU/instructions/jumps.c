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
