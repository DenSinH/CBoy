#include <stdint.h>

#include "PPU.h"
#include "../IO/IO.h"

#include "log.h"

#define GRAYSCALE_2BPP_TO_RGB(intensity) (intensity << 22) | (intensity << 14) | (intensity << 6)


void update_IO(s_PPU* ppu) {

}

void do_scanline(s_PPU* ppu, s_MEM* mem) {
    if (*ppu->scanline >= GB_HEIGHT)
        return;

    s_LCDC LCDC = CAST_REGISTER(s_LCDC, ppu->IO->registers[reg_LCDC]);
    uint16_t TileMapBaseAddress = LCDC.BGTileMapDisplay ? 0x1c00 : 0x1800;   // offset from VRAM start
    uint16_t TileDataBaseAddress = LCDC.BGWindowTileData ? 0x0000 : 0x0800;  // offset from VRAM start
    uint8_t Ty = (*ppu->scanline) >> 3; // / 8

    TileMapBaseAddress += Ty << 5;  // * 0x20

    uint8_t dy = *ppu->scanline & 7;
    uint8_t SliverLSB;
    uint8_t SliverMSB;
    int16_t TileID;
    uint8_t Color;

    for (int Tx = 0; Tx < (GB_WIDTH / 8); Tx++) {

        TileID = mem->VRAM[TileMapBaseAddress + Tx];
        if (!LCDC.BGWindowTileData) {
            if (TileID & 0x80) {
                TileID = 0xff00 | TileID;
            }
        }

        // tiles are 16 bytes in length, 2 bytes per sliver, so 2 * fine y offset
        SliverLSB = mem->VRAM[TileDataBaseAddress + (TileID << 4) + (dy << 1)];
        SliverMSB = mem->VRAM[TileDataBaseAddress + (TileID << 4) + (dy << 1) + 1];

        for (int dx = 0; dx < 8; dx++) {
            Color = ((SliverLSB >> (7 - dx)) & 1) | (((SliverMSB >> (7 - dx)) & 1) << 1);
            ppu->display[(*ppu->scanline) * GB_WIDTH + (Tx << 3) + dx] = GRAYSCALE_2BPP_TO_RGB(Color);
        }
    }

    update_IO(ppu);
}
