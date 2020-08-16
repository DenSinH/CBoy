#ifndef CBOY_DISPLAY_H
#define CBOY_DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

void display_init(char *title, int width, int height);
void display_close();
void blit_bitmap_32bppRGBA(uint32_t raw[], int width, int height);
void set_title(const char title[]);

#endif //CBOY_DISPLAY_H
