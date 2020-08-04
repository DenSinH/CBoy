#include "CPU.h"

#include "log.h"

void unimplemented(s_CPU* cpu, uint8_t instruction) {
    log_fatal("Unimplemlemented instruction: %x", instruction);
}

void init_cpu(s_CPU* cpu) {
    /* Populate the instruction tables */
    for (int i = 0; i < 0x100; i++) {
        cpu->prefixed[i] = unimplemented;
        cpu->unprefixed[i] = unimplemented;
    }
}