#include <stdio.h>

#include "src/System/GB.h"
#include "src/System/MEM/MEM.h"


int main(int argc, char** argv) {
    s_GB* GB = init_system();
    load_rom(&GB->mem, "roms/HelloWorld.gb");
    load_boot_rom(&GB->mem);

    run_trace(GB);

    return 0;
}
