#include <stdint.h>
#include <default.h>

#include "PPU.h"
#include "sprites.h"
#include "../MEM/MEM.h"

#include "log.h"

#define GRAYSCALE_2BPP_TO_RGB(intensity) (3 - (intensity)) * 0x00555555

void scan_oam(s_PPU* ppu, const s_LCDC LCDC) {
    ppu->number_of_sprites = 0;

    const uint8_t sprite_height = LCDC.OBJSize ? 16 : 8;
    uint8_t sprite_y;

    for (int i = 0; i < OAM_SIZE && ppu->number_of_sprites < 10; i += 4) {
        sprite_y = ppu->mem->OAM[i] - 16;

        if (sprite_y > *ppu->scanline || sprite_y + sprite_height <= *ppu->scanline) {
            continue;
        }

        ppu->sprites[ppu->number_of_sprites].y = sprite_y;
        ppu->sprites[ppu->number_of_sprites].x = ppu->mem->OAM[i + 1];
        ppu->sprites[ppu->number_of_sprites].Tid = ppu->mem->OAM[i + 2];
        if (sprite_height == 16) {
            ppu->sprites[ppu->number_of_sprites].Tid &= 0xfe;
        }
        ppu->sprites[ppu->number_of_sprites].attributes = CAST_ATTRIBUTES(ppu->mem->OAM[i + 3]);
        ppu->number_of_sprites++;
    }
}

static inline void draw_BG(s_PPU* ppu, s_MEM* mem, const s_LCDC LCDC) {
    uint16_t TileMapBaseAddress = LCDC.BGTileMapDisplay ? 0x1c00 : 0x1800;   // offset from VRAM start
    uint16_t TileDataBaseAddress = LCDC.BGWindowTileData ? 0x0000 : 0x0800;  // offset from VRAM start
    const uint8_t Ty = (*ppu->scanline) >> 3; // / 8

    TileMapBaseAddress += Ty << 5;  // * 0x20

    const uint8_t dy = *ppu->scanline & 7;
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
            ppu->BG_pixels[(Tx << 3) + dx] = Color;
        }
    }
}

static inline void draw_OBJ(s_PPU* ppu, s_MEM* mem, s_LCDC LCDC) {
    // clear memory to avoid confusion
    memset(ppu->sprite_prio, 0, sizeof(ppu->sprite_prio));
    memset(ppu->sprite_pixels, 0, sizeof(ppu->sprite_pixels));

    uint8_t base_x;
    uint8_t dy;
    s_attributes attributes;

    uint8_t SliverLSB;
    uint8_t SliverMSB;
    uint8_t Color;

    // loop over sprites in reverse order to draw them with the right priority
    for (int i = ppu->number_of_sprites - 1; i >= 0; i--) {
        // get sprite data
        base_x = ppu->sprites[i].x;

        // y <= scanline < y + sprite_height
        dy = (*ppu->scanline) - ppu->sprites[i].y;
        attributes = ppu->sprites[i].attributes;

        if (attributes.Yflip) {
            dy = (LCDC.OBJSize ? 15 : 7) - dy;
        }


        // get sprite sliver
        SliverLSB = mem->VRAM[(ppu->sprites[i].Tid << 4) + (dy << 1)];
        SliverMSB = mem->VRAM[(ppu->sprites[i].Tid << 4) + (dy << 1) + 1];

        if (attributes.Xflip) {
            SliverLSB = flip_byte(SliverLSB);
            SliverMSB = flip_byte(SliverMSB);
        }

        for (int dx = 0; dx < 8; dx++) {
            if ((base_x + dx) >= 0 && (base_x + dx < GB_WIDTH)) {
                Color = ((SliverLSB >> (7 - dx)) & 1) | (((SliverMSB >> (7 - dx)) & 1) << 1);
                ppu->sprite_pixels[ppu->sprites[i].x + dx] = Color;
                ppu->sprite_prio[ppu->sprites[i].x + dx] = attributes.priority;
                // todo: OBJ palette
            }
        }
    }
}

void do_scanline(s_PPU* ppu, s_MEM* mem) {
    if (*ppu->scanline >= GB_HEIGHT)
        return;

    s_LCDC LCDC = CAST_VAL_TO_REGISTER(s_LCDC, ppu->IO->registers[reg_LCDC]);

    scan_oam(ppu, LCDC);

    draw_BG(ppu, mem, LCDC);
    draw_OBJ(ppu, mem, LCDC);

    uint8_t Color;
    unsigned int scanline_offset = GB_WIDTH * (*ppu->scanline);
    const uint8_t Palette = ppu->IO->registers[reg_BGP];

    for (int x = 0; x < GB_WIDTH; x++) {
        Color = ppu->BG_pixels[x];
        if (ppu->sprite_prio[x] || Color == 0) {
            Color = ppu->sprite_pixels[x];
        }
        ppu->display[scanline_offset + x] = GRAYSCALE_2BPP_TO_RGB((Palette >> (Color << 1)) & 0x03);
    }
}
