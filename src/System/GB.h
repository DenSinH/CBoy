#ifndef CBOY_GB_H
#define CBOY_GB_H

#include <stdbool.h>

#include "Display/display.h"
#include "CPU/CPU.h"
#include "MEM/MEM.h"
#include "PPU/PPU.h"

#define FRAME_CAP

#define GB_DOTS_PER_SCANLINE 456
#define GB_VBLANK_LINES 10

typedef struct {
    s_CPU cpu;
    s_MEM mem;
    s_IO IO;
    s_PPU ppu;

    bool shut_down;
} s_GB;

s_GB* init_system();

void run(s_GB* GB);
void run_trace(s_GB* GB, char log_file[]);

#endif //CBOY_GB_H
