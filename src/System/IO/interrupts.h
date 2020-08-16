#ifndef CBOY_INTERRUPTS_H
#define CBOY_INTERRUPTS_H

#include "log.h"

#include "IO.h"
#include <stdbool.h>

typedef enum Interrupt {
    Interrupt_VBlank  = 0x01,
    Interrupt_LCDStat = 0x02,
    Interrupt_Timer   = 0x04,
    Interrupt_Serial  = 0x08,
    Interrupt_Joypad  = 0x10,
} Interrupt;

typedef union s_InterruptRegister {
    struct {
        bool VBlank:    1;
        bool LCDStat:   1;
        bool Timer:     1;
        bool Serial:    1;
        bool Joypad:    1;
    };
} s_InterruptRegister;

static inline void request_interrupt(s_IO* IO, Interrupt interrupt) {
    log("Interrupt %x requested", interrupt);
    IO->registers[reg_IF] |= interrupt;
}

static inline bool interrupt_enabled(s_IO* IO) {
    return (IO->IE & IO->registers[reg_IF]) != 0;
}

#endif //CBOY_INTERRUPTS_H
