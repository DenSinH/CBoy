#include <stdio.h>

#include "src/System/GB.h"
#include "src/System/MEM/MEM.h"

int main(int argc, char** argv) {
    s_GB* GB = init_system();
    load_rom(&GB->mem, "roms/HelloWorld.gb");

    for (int i = 0; i < 0x8000; i++) {
        printf("%x", read_byte(&GB->mem, i));
        if ((i & 0xf) == 0) {
            printf("\n");
        }
    }
    // run(GB);

    return 0;
}
