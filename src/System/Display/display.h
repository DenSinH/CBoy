#ifndef CBOY_DISPLAY_H
#define CBOY_DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

void init_display(char title[], int width, int height);
void close_display();
void blit_bitmap_32bppRGBA(uint32_t raw[], int width, int height);

#endif //CBOY_DISPLAY_H
