#ifndef CBOY_GB_H
#define CBOY_GB_H

#include "CPU/CPU.h"
#include "MEM/MEM.h"
#include "PPU/PPU.h"

typedef struct {
    s_CPU cpu;
    s_MEM mem;
    // todo: PPU
} s_GB;

s_GB* init_system();

void run(s_GB* GB);
void run_trace(s_GB* GB, char log_file[]);

#endif //CBOY_GB_H
