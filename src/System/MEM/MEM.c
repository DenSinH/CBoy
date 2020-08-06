#include <stdlib.h>

#include "MEM.h"

#include "default.h"
#include "log.h"

#define BOOT_ROM_FILE "./src/System/MEM/dmg_boot.bin"

void load_boot_rom(s_MEM* mem) {
    load_rom(mem, BOOT_ROM_FILE);
    mem->IO[0x44] = 0x90;       // VY
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
/*
 * perform action on memory location based on address
 */
#define ADDRESS_MAP(mem, address, action, IO_action, IE_action)            \
    switch(address >> 12) {                                                \
        case 0x0 ... 0x3:                                                  \
            /* ROM bank 00 */                                              \
            action(mem->ROM_b00, address)                                  \
            break;                                                         \
        case 0x4 ... 0x7:                                                  \
            /*  ROM bank 01 -> NN */                                       \
            action(mem->ROM_bNN, address & 0x3fff)                         \
            break;                                                         \
        case 0x8 ... 0x9:                                                  \
            /*  VRAM */                                                    \
            action(mem->VRAM, address & 0x1fff)                            \
            break;                                                         \
        case 0xa ... 0xb:                                                  \
            /*  eRAM */                                                    \
            action(mem->eRAM, address & 0x1fff)                            \
            break;                                                         \
        case 0xc:                                                          \
        case 0xe: /*  echo RAM */                                          \
            /*  WRAM bank 0 */                                             \
            action(mem->WRAM_b0, address & 0x0fff)                         \
            break;                                                         \
        case 0xd:                                                          \
            /*  WRAM bank 1 -> N */                                        \
            action(mem->WRAM_bN, address & 0x0fff)                         \
            break;                                                         \
        case 0xf:                                                          \
            if (address < 0xFDFF) {                                        \
                /*  echo RAM */                                            \
                action(mem->WRAM_bN, address & 0x0fff)                     \
                break;                                                     \
            }                                                              \
            else if (address < 0xFEA0) {                                   \
                /*  OAM */                                                 \
                action(mem->OAM, address & 0x00ff)                         \
                break;                                                     \
            }                                                              \
            else if (address < 0xFF00) {                                   \
                /*  not usable */                                          \
                /*  todo: open bus? */                                     \
                log_fatal("unused section access")                         \
                break;                                                     \
            }                                                              \
            else if (address < 0xFF80) {                                   \
                /*  IO (todo) */                                           \
                IO_action(mem->IO, address & 0x00ff)                       \
                break;                                                     \
            }                                                              \
            else if (address < 0xFFFF) {                                   \
                /*  HRAM */                                                \
                action(mem->HRAM, address & 0x007f)                        \
                break;                                                     \
            }                                                              \
            else {                                                         \
                /*  IE */                                                  \
                IE_action(mem->IE, 0)                                      \
                break;                                                     \
            }                                                              \
        default:                                                           \
        log_fatal("Invalid memory address: %x", address);                  \
    }

#define READ_BYTE(section, address) return section[address];
#define READ_IO(section, address) READ_BYTE(section, address);
#define READ_IE(section, address) return section;
uint8_t read_byte(s_MEM* mem, uint16_t address) {
    /* read single byte from memory */
    ADDRESS_MAP(mem, address, READ_BYTE, READ_IO, READ_IE);
}

uint16_t read_short(s_MEM* mem, uint16_t address) {
    /* read short from memory */
    return (uint16_t)(read_byte(mem, address) | (read_byte(mem, address + 1) << 8));
}


#define WRITE_BYTE(section, address) section[address] = value;
#define WRITE_IO(section, address) WRITE_BYTE(section, address);
#define WRITE_IE(section, address) section = value;
void write_byte(s_MEM* mem, uint16_t address, uint8_t value) {
    /* write single byte to memory */
    ADDRESS_MAP(mem, address, WRITE_BYTE, WRITE_IO, WRITE_IE)
}

void write_short(s_MEM* mem, uint16_t address, uint16_t value) {
    /* write short to memory */
    write_byte(mem, address, value & 0xff);
    write_byte(mem, address + 1, value >> 8);
}

