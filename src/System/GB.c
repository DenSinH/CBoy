#include <stdlib.h>
#include <mem.h>
#include <stdbool.h>

#include "GB.h"

s_GB* init_system() {
    s_GB* GB = malloc(sizeof(s_GB));
    memset(GB, 0x00, sizeof(s_GB));

    GB->cpu.mem = &(GB->mem);

    return GB;
}

void run(s_GB* GB) {
    bool shut_down = false;

    while (!shut_down) {
        // step CPU
    }
}
