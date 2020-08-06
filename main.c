#include <stdio.h>

#include "src/System/GB.h"
#include "src/System/MEM/MEM.h"

#define LOG_FILE "./logs/hello_world.log"

int main(int argc, char** argv) {
    s_GB* GB = init_system();
    load_rom(&GB->mem, "roms/cpu_instrs/individual/01-special.gb");

    skip_boot_rom(&GB->cpu);
    // load_boot_rom(&GB->mem);

    run(GB);
    // run_trace(GB, LOG_FILE);

    return 0;
}
