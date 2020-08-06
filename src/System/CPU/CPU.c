
#include "CPU.h"
#include "../MEM/MEM.h"

void skip_boot_rom(s_CPU* cpu) {
    cpu->registers[r8_B] = 0x00;
    cpu->registers[r8_C] = 0x13;
    cpu->registers[r8_D] = 0x00;
    cpu->registers[r8_E] = 0xd8;
    cpu->registers[r8_H] = 0x01;
    cpu->registers[r8_L] = 0x4d;
    cpu->registers[r8_A] = 0x01;
    cpu->flags = 0xb0;
    cpu->SP = 0xfffe;
    cpu->PC = 0x0100;
}

int cpu_step(s_CPU* cpu) {
    uint8_t instruction = read_byte(cpu->mem, cpu->PC++);

    if (instruction == 0xCB) {
        // prefixed instruction
        instruction = read_byte(cpu->mem, cpu->PC++);
        return cpu->prefixed[instruction](cpu, instruction);
    }
    else {
        // unprefixed instruction
        return cpu->unprefixed[instruction](cpu, instruction);
    }
}