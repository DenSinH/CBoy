#include <stdlib.h>
#include <stdio.h>
#include <mem.h>
#include <stdbool.h>

#include "GB.h"
#include "CPU/CPU.h"

#include "log.h"

s_GB* init_system() {
    s_GB* GB = malloc(sizeof(s_GB));
    memset(GB, 0x00, sizeof(s_GB));

    GB->cpu.mem = &(GB->mem);
    init_cpu(&GB->cpu);

    return GB;
}

void run(s_GB* GB) {
    /* run the GB system (to be called after ROM is loaded) */
    bool shut_down = false;

    while (!shut_down) {
        step(&GB->cpu);
    }
}

#define LOG_FILE "./logs/boot_rom.log"

void run_trace(s_GB* GB) {
    /* run the GB system vs a trace generated with mGBA (to be called after ROM is loaded) */

    // load file and initialize string variables
    FILE* fp;
    char* mGBA = NULL;
    char mine[LOG_LINE_LENGTH + 1];
    size_t len = 0;
    ssize_t read;

    fp = fopen(LOG_FILE, "r");
    if (fp == NULL)
        log_fatal("Could not open file with file path: " LOG_FILE);

    bool shut_down = false;

    while ((!shut_down) && ((read = getline(&mGBA, &len, fp)) != -1)) {

        // read next line and report them
        mGBA[read - 1] = ' ';         // remove newline character
        log_debug("mGBA: %s", mGBA);

        mGBA_log_format(&GB->cpu, mine);
        log_debug("mine: %s", mine);

        // compare lines
        if (strncmp(mine, mGBA, LOG_LINE_LENGTH) != 0) {
            log_warn("mGBA: %s", mGBA);
            log_warn("mine: %s", mine);
            log_warn("found error in line, press key to continue...");
            getchar();
        }

        step(&GB->cpu);
    }
}