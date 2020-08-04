#include <stdlib.h>
#include <mem.h>
#include <stdbool.h>

#include "GB.h"
#include "CPU/CPU.h"

s_GB* init_system() {
    s_GB* GB = malloc(sizeof(s_GB));
    memset(GB, 0x00, sizeof(s_GB));

    GB->cpu.mem = &(GB->mem);
    init_cpu(&GB->cpu);

    return GB;
}

void run(s_GB* GB) {
    bool shut_down = false;

    while (!shut_down) {
        step(&GB->cpu);
    }
}
