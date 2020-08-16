#include <stdlib.h>
#include <stdio.h>
#include <mem.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <time.h>

#include "GB.h"
#include "CPU/CPU.h"
#include "Display/display.h"

#include "log.h"

#define LOG_CPU

#ifdef FRAME_CAP
const struct timespec frame_delay = {.tv_sec = 0, .tv_nsec = 16000000 };  // 16ms
#endif

s_GB* init_system() {
    s_GB* GB = malloc(sizeof(s_GB));
    memset(GB, 0x00, sizeof(s_GB));

    GB->cpu.mem = &(GB->mem);
    GB->ppu.mem = &(GB->mem);
    GB->ppu.scanline = &GB->IO.registers[reg_LY];
    GB->cpu.IO = GB->mem.IO = GB->ppu.IO = &(GB->IO);
    cpu_init(&GB->cpu);
    display_init("CBoy", GB_WIDTH * GB_SCALE, GB_HEIGHT * GB_SCALE);

    return GB;
}

void handle_events(s_GB* GB) {
    SDL_Event e;
    char log[LOG_LINE_LENGTH + 1];
    while (SDL_PollEvent( &e ) != 0) {
        switch (e.type) {
            case SDL_QUIT:
                GB->shut_down = true;
                break;
            case SDL_KEYDOWN:
                switch (SDL_GetKeyFromScancode(e.key.keysym.scancode)) {
                    case GB_SDL_BUTTON_LEFT_KEYSYM:
                        GB->IO.joypad.Left = true;
                        break;
                    case GB_SDL_BUTTON_RIGHT_KEYSYM:
                        GB->IO.joypad.Right = true;
                        break;
                    case GB_SDL_BUTTON_DOWN_KEYSYM:
                        GB->IO.joypad.Down = true;
                        break;
                    case GB_SDL_BUTTON_UP_KEYSYM:
                        GB->IO.joypad.Up = true;
                        break;
                    case 'z':
                        GB->IO.joypad.A = true;
                        break;
                    case 'x':
                        GB->IO.joypad.B = true;
                        break;
                    case 'a':
                        GB->IO.joypad.Start = true;
                        break;
                    case 's':
                        GB->IO.joypad.Select = true;
                        break;

                    case 'v':
                        for (int i = 0; i < sizeof(GB->mem.VRAM); i++) {
                            if ((i & 0x1f) == 0) {
                                printf("\n %04x: ", i);
                            }
                            printf("%02x ", GB->mem.VRAM[i]);
                        }
                        break;
                    case 'i':
                        mGBA_log_format(&GB->cpu, log);
                        printf("%s\n", log);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (SDL_GetKeyFromScancode(e.key.keysym.scancode)) {
                    case GB_SDL_BUTTON_LEFT_KEYSYM:
                        GB->IO.joypad.Left = false;
                        break;
                    case GB_SDL_BUTTON_RIGHT_KEYSYM:
                        GB->IO.joypad.Right = false;
                        break;
                    case GB_SDL_BUTTON_DOWN_KEYSYM:
                        GB->IO.joypad.Down = false;
                        break;
                    case GB_SDL_BUTTON_UP_KEYSYM:
                        GB->IO.joypad.Up = false;
                        break;
                    case 'z':
                        GB->IO.joypad.A = false;
                        break;
                    case 'x':
                        GB->IO.joypad.B = false;
                        break;
                    case 'a':
                        GB->IO.joypad.Start = false;
                        break;
                    case 's':
                        GB->IO.joypad.Select = false;
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    }
}

void do_frontend(s_GB* GB) {
    blit_bitmap_32bppRGBA(GB->ppu.display, GB_WIDTH, GB_HEIGHT);
    handle_events(GB);
}

void run(s_GB* GB) {
    /* run the GB system (to be called after ROM is loaded) */
    int cycles = 0;
    char status[LOG_LINE_LENGTH + 1];

    while (!GB->shut_down) {
        // 4 cycles (1 t cycle) per dot
        while (cycles < GB_DOTS_PER_SCANLINE * 4) {
#ifdef LOG_CPU
            if (GB->cpu.freeze) {
                mGBA_log_format(&GB->cpu, status);
                log_debug("%s", status);
                getchar();
            }
#endif

            cycles += cpu_step(&GB->cpu);
        }
        cycles -= GB_DOTS_PER_SCANLINE * 4;

        (*GB->ppu.scanline)++;
        if (*GB->ppu.scanline == GB_HEIGHT) {
            // VBlank start
            do_frontend(GB);
#ifdef FRAME_CAP
            nanosleep(&frame_delay, NULL);
#endif
        }
        else if (*GB->ppu.scanline == GB_HEIGHT + GB_VBLANK_LINES) {
            *GB->ppu.scanline = 0;
        }
        do_scanline(&GB->ppu, &GB->mem);
    }

    display_close();
}

bool ignore_case(s_GB* GB) {
    return false;
}

bool read_line(s_GB* GB) {
    return true;
}

void run_trace(s_GB* GB, char log_file[]) {
    /* run the GB system vs a trace generated with mGBA (to be called after ROM is loaded) */

    // load file and initialize string variables
    FILE* fp;
    char* mGBA = NULL;
    char mine[LOG_LINE_LENGTH + 1];
    size_t len = 0;
    ssize_t read;

    fp = fopen(log_file, "r");
    if (fp == NULL)
        log_fatal("Could not open file with file path: %s", log_file);

    int cycles = 0;

    while ((!GB->shut_down) && (!read_line(GB) || ((read = getline(&mGBA, &len, fp)) != -1))) {

        // read next line and report them
        mGBA[read - 1] = ' ';         // remove newline character
        log_debug("mGBA: %s", mGBA);

        mGBA_log_format(&GB->cpu, mine);
        log_debug("mine: %s", mine);

        // compare lines
        if (read_line(GB) && !ignore_case(GB) && strncmp(mine, mGBA, LOG_LINE_LENGTH) != 0) {
            log_warn("mGBA: %s", mGBA);
            log_warn("mine: %s", mine);
            log_warn("found error in line, press key to continue...");
            getchar();
        }

        cycles += cpu_step(&GB->cpu);

        if (cycles > GB_DOTS_PER_SCANLINE * 4) {
            cycles -= GB_DOTS_PER_SCANLINE;
            (*GB->ppu.scanline)++;

            if ((*GB->ppu.scanline) == GB_HEIGHT) {
                // VBlank
                do_frontend(GB);

                // we'll just unlimit the framerate if we're log diffing anyway
            }
            else if ((*GB->ppu.scanline) == GB_HEIGHT + GB_VBLANK_LINES) {
                (*GB->ppu.scanline) = 0;
            }
            do_scanline(&GB->ppu, &GB->mem);
        }

        if (GB->cpu.PC == 0xC820) {
            log_warn("Should have written to D81B?")
        }
    }

    display_close();
}