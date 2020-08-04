#include <stdlib.h>

#include "MEM.h"

#include "default.h"
#include "log.h"

#define BOOT_ROM_FILE "./src/System/MEM/dmg_boot.bin"

void load_boot_rom(s_MEM* mem) {
    load_rom(mem, BOOT_ROM_FILE);
}

void load_rom(s_MEM* mem, char file_name[]) {
    FILE* file = fopen(file_name, "rb");
    if (!file) log_fatal("failed read");

    fseek(file, 0, SEEK_END);
    int f_size = ftell(file);
    rewind(file);

    // read first bytes
    fread(mem->ROM_b00, 1, MIN(f_size, sizeof(mem->ROM_b00)), file);
    // read last bytes, if any leftover
    fread(mem->ROM_bNN, 1, MAX(0, f_size - sizeof(mem->ROM_b00)), file);
    fclose(file);

    log("Read %i bytes", f_size);
}

struct address_data {
    uint8_t *section;
    uint16_t masked_address;
};

struct address_data get_address_data__(s_MEM* mem, uint16_t address) {
    struct address_data data = {};

    switch(address >> 12) {
        case 0x0 ... 0x3:
            // ROM bank 00
            data.section = mem->ROM_b00;
            data.masked_address = address;
            break;
        case 0x4 ... 0x7:
            // ROM bank 01 -> NN
            data.section = mem->ROM_bNN;
            data.masked_address = address & 0x3fff;
            break;
        case 0x8 ... 0x9:
            // VRAM
            data.section = mem->VRAM;
            data.masked_address = address & 0x1fff;
            break;
        case 0xa ... 0xb:
            // eRAM
            data.section = mem->eRAM;
            data.masked_address = address & 0x1fff;
            break;
        case 0xc:
        case 0xe: // echo RAM
            // WRAM bank 0
            data.section = mem->WRAM_b0;
            data.masked_address = address & 0x0fff;
            break;
        case 0xd:
            // WRAM bank 1 -> N
            data.section = mem->WRAM_bN;
            data.masked_address = address & 0x0fff;
            break;
        case 0xf:
            if (address < 0xFDFF) {
                // echo RAM
                data.section = mem->WRAM_bN;
                data.masked_address = address & 0x0fff;
                break;
            }
            else if (address < 0xFEA0) {
                // OAM
                data.section = mem->OAM;
                data.masked_address = address & 0x00ff;
                break;
            }
            else if (address < 0xFF00) {
                // not usable
                // todo: open bus?
                data.section = NULL;
                data.masked_address = 0;
                break;
            }
            else if (address < 0xFF80) {
                // IO (todo)
                data.section = mem->IO;
                data.masked_address = address & 0x00ff;
                break;
            }
            else if (address < 0xFFFF) {
                // HRAM
                data.section = mem->HRAM;
                data.masked_address = address & 0x007f;
                break;
            }
            else {
                // IE
                data.section = &mem->IE;
                data.masked_address = 0;
                break;
            }
        default:
        log_fatal("Invalid memory address: %x", address);
    }
    return data;
}

uint8_t read_byte(s_MEM* mem, uint16_t address) {
    struct address_data data = get_address_data__(mem, address);
    return data.section[data.masked_address];
}

uint16_t read_short(s_MEM* mem, uint16_t address) {
    return ((uint16_t)read_byte(mem, address)) | (((uint16_t)read_byte(mem, address + 1)) << 8);
}

void write_byte(s_MEM* mem, uint16_t address, uint8_t value) {
    struct address_data data = get_address_data__(mem, address);
    data.section[data.masked_address] = value;
}

void write_short(s_MEM* mem, uint16_t address, uint16_t value) {
    write_byte(mem, address, value & 0xff);
    write_byte(mem, address + 1, value >> 8);
}

