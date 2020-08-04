#include "CPU.h"

#include "instructions/load.c"

#include "log.h"

void unimplemented_unprefixed(s_CPU* cpu, uint8_t instruction) {
    log_fatal("Unimplemlemented unprefixed instruction: %x", instruction);
}

void unimplemented_prefixed(s_CPU* cpu, uint8_t instruction) {
    log_fatal("Unimplemlemented prefixed instruction: %x", instruction);
}

void init_cpu(s_CPU* cpu) {
    /* Populate the instruction tables */
    for (int instruction = 0; instruction < 0x100; instruction++) {
        if ((instruction & 0xcf) == 0x01) {
            // 00xx 0001
            cpu->unprefixed[instruction] = LD_u16;
        }
        else {
            cpu->unprefixed[instruction] = unimplemented_unprefixed;
        }
    }

    for (int i = 0; i < 0x100; i++) {
        cpu->prefixed[i] = unimplemented_prefixed;
    }
}