#include <stdio.h>

#include "src/System/GB.h"
#include "src/System/MEM/MEM.h"

#define LOG_FILE "./logs/EpicLog_blargg8.txt"

int main(int argc, char** argv) {
    s_GB* GB = init_system();
    load_rom(&GB->mem, "roms/tests/cpu_instrs/individual/10-bit ops.gb");

    skip_boot_rom(&GB->cpu);
    // load_boot_rom(&GB->mem);

    run(GB);
    // run_trace(GB, LOG_FILE);

    return 0;
}
