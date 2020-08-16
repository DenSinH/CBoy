#include <stdio.h>

#include "src/System/GB.h"
#include "src/System/MEM/MEM.h"

#define LOG_FILE "./logs/r,imm.log"

int main(int argc, char** argv) {
    s_GB* GB = init_system();
    // load_rom(&GB->mem, "roms/tests/cpu_instrs/individual/11-op a,(hl).gb");
    // load_rom(&GB->mem, "roms/tests/optix/fairylake.gb");
    load_rom(&GB->mem, "roms/tetris.gb");

    skip_boot_rom(&GB->cpu);
    // load_boot_rom(&GB->mem);

    run(GB);
    // run_trace(GB, LOG_FILE);

    return 0;
}
