#include "CPU.h"

#include "instructions/stack.c"
#include "instructions/load.c"
#include "instructions/arithmetic.c"
#include "instructions/bit_op.c"
#include "instructions/shift.c"
#include "instructions/jumps.c"

#include "log.h"

#define DO_SELF_TEST

void unimplemented_unprefixed(s_CPU* cpu, uint8_t instruction) {
    log_fatal("Unimplemlemented unprefixed instruction: %02x", instruction);
}

void unimplemented_prefixed(s_CPU* cpu, uint8_t instruction) {
    log_fatal("Unimplemlemented prefixed instruction: %02x", instruction);
}

void init_cpu(s_CPU* cpu) {
    /* Populate the instruction tables */
    for (int instruction = 0; instruction < 0x100; instruction++) {
        /* ========================================================
         *                      TOP 4 ROWS
         * ========================================================
         */
        // col 0
        if ((instruction > 0x10) && (instruction & 0xc7) == 0x00) {
            // > 0x10 AND 00xx x000
            cpu->unprefixed[instruction] = JP_cc_offset;
        }
        // col 1
        else if ((instruction & 0xcf) == 0x01) {
            // 00xx 0001
            cpu->unprefixed[instruction] = LD_r16_u16;
        }
        // col 2
        else if ((instruction & 0xcf) == 0x02) {
            // 00xx 0010
            cpu->unprefixed[instruction] = LD_r16_A;
        }
        // col 3
        else if ((instruction & 0xcf) == 0x03) {
            // 00xx 0011
            cpu->unprefixed[instruction] = INC_r16;
        }
        // col 4 / C
        else if ((instruction & 0xc7) == 0x04) {
            // 00xx x100
            cpu->unprefixed[instruction] = INC_r8;
        }
        // col 5 / D
        else if ((instruction & 0xc7) == 0x05) {
            // 00xx x100
            cpu->unprefixed[instruction] = DEC_r8;
        }
        // col 6 / E
        else if ((instruction & 0xc7) == 0x06) {
            // 00xx x110
            cpu->unprefixed[instruction] = LD_r8_u8;
        }
        // col 7 / F
        else if ((instruction & 0xe7) == 0x07) {
            // 000X X111
            switch (instruction) {
                case 0x07:
                    cpu->unprefixed[instruction] = RLCA;
                    break;
                case 0x17:
                    cpu->unprefixed[instruction] = RLA;
                    break;
                case 0x0f:
                    cpu->unprefixed[instruction] = RRCA;
                    break;
                case 0x1f:
                    cpu->unprefixed[instruction] = RRA;
                    break;
                default:
                log_fatal("Invalid instruction when decoding");
            }
        }
        // col 8
        // col 9
        // col A
        else if ((instruction & 0xcf) == 0x0a) {
            // 00xx a
            cpu->unprefixed[instruction] = LD_A_r16;
        }
        // col B
        else if ((instruction & 0xcf) == 0x0b) {
            // 00xx 1011
            cpu->unprefixed[instruction] = DEC_r16;
        }

        /* ========================================================
         *                      SECOND 4 ROWS
         * ========================================================
         */
        /* order is important in the next 3 decodings: */
        else if ((instruction & 0xf8) == 0x70) {
            // 7 0xxx
            cpu->unprefixed[instruction] = LD_HL_r8_HALT;
        }
        else if ((instruction & 0xc7) == 0x46) {
            // 01xx x110
            cpu->unprefixed[instruction] = LD_r8_HL;
        }
        else if ((instruction & 0xc0) == 0x40) {
            // 7 1xxx
            cpu->unprefixed[instruction] = LD_r8_r8;
        }
        /* ========================================================
        *                      THIRD 4 ROWS
        * ========================================================
        */
        /* order is important for the next 2 decodings */
        else if ((instruction & 0xc7) == 0x86) {
            // 10XX X110
            cpu->unprefixed[instruction] = ARITH_A_HL;
        }
        else if ((instruction & 0xc0) == 0x80) {
            // 10XX XRRR
            cpu->unprefixed[instruction] = ARITH_A_r8;
        }

        /* ========================================================
        *                      BOTTOM 4 ROWS
        * ========================================================
        */
        /* order is important for the next 2 decodings */
        // col 1
        else if ((instruction & 0xcf) == 0xc1) {
            // 11XX 0001
            cpu->unprefixed[instruction] = POP_r16;
        }
        // col 0 / 8 / 9
        else if ((instruction & 0xe6) == 0xc0) {
            // 110X X00X
            cpu->unprefixed[instruction] = RET_cc;
        }
        // col 0 / 2
        else if ((instruction & 0xed) == 0xe0) {
            // 111x 00x0
            cpu->unprefixed[instruction] = LD_FF00_A;
        }
        // col 3
        // col 4 / C / D
        else if ((instruction & 0xe7) == 0xc4 || (instruction == 0xcd)) {
            // 110x x100 OR cd
            cpu->unprefixed[instruction] = CALL_cc;
        }
        // col 5
        else if ((instruction & 0xcf) == 0xc5) {
            // 11XX 0101
            cpu->unprefixed[instruction] = PUSH_r16;
        }
        // col 6 / E
        else if ((instruction & 0xc7) == 0xc6) {
            // 11XX X110
            cpu->unprefixed[instruction] = ARITH_A_u8;
        }
        // col 7
        // col A
        else if ((instruction & 0xEF) == 0xEA) {
            // 111X A
            cpu->unprefixed[instruction] = LD_u16_A;
        }
        // col B
        // col F
        else {
            cpu->unprefixed[instruction] = unimplemented_unprefixed;
        }
    }

    for (int instruction = 0; instruction < 0x100; instruction++) {
        if ((instruction & 0xf8) == 0x00) {
            // 0000 0XXX
            cpu->prefixed[instruction] = RLC_r8;
        }
        else if ((instruction & 0xf8) == 0x08) {
            // 0000 1XXX
            cpu->prefixed[instruction] = RRC_r8;
        }
        else if ((instruction & 0xf8) == 0x10) {
            // 0001 0XXX
            cpu->prefixed[instruction] = RL_r8;
        }
        else if ((instruction & 0xf8) == 0x18) {
            // 0001 1XXX
            cpu->prefixed[instruction] = RR_r8;
        }
        else if ((instruction & 0xc0) == 0x40) {
            // 01XX XXXX
            cpu->prefixed[instruction] = BIT_u3_r8;
        }
        else {
            cpu->prefixed[instruction] = unimplemented_prefixed;
        }
    }

#ifdef DO_SELF_TEST
    /* ========================================================
     *                      SELF TEST
     * ========================================================
     */
    for (int i = 0; i < 0x100; i++) {
        if (cpu->unprefixed[i] == NULL) {
            log_fatal("Unprefixed instruction %x is NULL pointer!", i);
        }

        if (cpu->prefixed[i] == NULL) {
            log_fatal("Prefixed instruction %x is NULL pointer!", i);
        }
    }
#endif
}