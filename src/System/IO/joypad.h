#ifndef CBOY_JOYPAD_H
#define CBOY_JOYPAD_H

#include <stdbool.h>
#include <stdint.h>

#define GB_SDL_BUTTON_LEFT_KEYSYM 0x40000050
#define GB_SDL_BUTTON_RIGHT_KEYSYM 0x4000004f
#define GB_SDL_BUTTON_UP_KEYSYM 0x40000052
#define GB_SDL_BUTTON_DOWN_KEYSYM 0x40000051

enum JOYP {
    JOYP_direction_keys = 0x10,
    JOYP_button_keys = 0x20
} JOYP;

typedef union s_JOYPAD {
    struct {
        bool Right:    1;
        bool Left:     1;
        bool Up:       1;
        bool Down:     1;
        bool A:        1;
        bool B:        1;
        bool Select:   1;
        bool Start:    1;
    };
} s_JOYPAD;

uint8_t button_keys(s_JOYPAD* joypad);
uint8_t direction_keys(s_JOYPAD* joypad);

#endif //CBOY_JOYPAD_H
