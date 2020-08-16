#include "joypad.h"

#include <stdio.h>

uint8_t button_keys(s_JOYPAD* joypad) {
    return ((uint8_t)~(*(uint8_t*)joypad)) >> 4;
}

uint8_t direction_keys(s_JOYPAD* joypad) {
    return (~(*(uint8_t*)joypad)) & 0x0f;
}