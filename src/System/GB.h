#ifndef CBOY_GB_H
#define CBOY_GB_H

#include "CPU/Z80.h"
#include "MEM/MEM.h"
#include "PPU/PPU.h"

typedef struct {
    s_Z80 cpu;
    s_MEM mem;
    // todo: PPU
} s_GB;

s_GB* init_system();
void run(s_GB* GB);

#endif //CBOY_GB_H
