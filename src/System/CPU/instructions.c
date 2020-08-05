#include "CPU.h"

#include "instructions/load.c"
#include "instructions/arithmetic.c"
#include "instructions/bit_op.c"
#include "instructions/jumps.c"

#include "log.h"

void unimplemented_unprefixed(s_CPU* cpu, uint8_t instruction) {
    log_fatal("Unimplemlemented unprefixed instruction: %02x", instruction);
}

void unimplemented_prefixed(s_CPU* cpu, uint8_t instruction) {
    log_fatal("Unimplemlemented prefixed instruction: %02x", instruction);
}

void init_cpu(s_CPU* cpu) {
    /* Populate the instruction tables */
    for (int instruction = 0; instruction < 0x100; instruction++) {
        if ((instruction > 0x10) && (instruction & 0xc7) == 0x00) {
            // > 0x10 AND 00xx x000
            cpu->unprefixed[instruction] = JP_cc;
        }
        else if ((instruction & 0xcf) == 0x01) {
            // 00xx 0001
            cpu->unprefixed[instruction] = LD_r16_u16;
        }
        else if ((instruction & 0xcf) == 0x02) {
            // 00xx 0010
            cpu->unprefixed[instruction] = LD_r16_A;
        }
        else if ((instruction & 0xc7) == 0x06) {
            // 00xx x110
            cpu->unprefixed[instruction] = LD_r8_u8;
        }
        else if ((instruction & 0xf8) == 0x78) {
            // 7 1xxx
            cpu->unprefixed[instruction] = LD_A_r8;
        }
        else if ((instruction & 0xf8) == 0xa8) {
            // A 1xxx
            cpu->unprefixed[instruction] = XOR_A_r8;
        }
        else if ((instruction & 0xed) == 0xe0) {
            // 111x 00x0
            cpu->unprefixed[instruction] = LD_C_A;
        }
        else if ((instruction & 0xc7) == 0x04) {
            // 00xx x100
            cpu->unprefixed[instruction] = INC_r8;
        }
        else {
            cpu->unprefixed[instruction] = unimplemented_unprefixed;
        }
    }

    for (int instruction = 0; instruction < 0x100; instruction++) {
        if ((instruction & 0xc0) == 0x40) {
            // 01XX XXXX
            cpu->prefixed[instruction] = BIT_u3_r8;
        }
        else {
            cpu->prefixed[instruction] = unimplemented_prefixed;
        }
    }
}