
#include "CPU.h"
#include "../MEM/MEM.h"

void step(s_CPU* cpu) {
    uint8_t instruction = read_byte(cpu->mem, cpu->PC++);

    if (instruction == 0xCB) {
        // prefixed instruction
        instruction = read_byte(cpu->mem, cpu->PC++);
        cpu->prefixed[instruction](cpu, instruction);
    }
    else {
        cpu->prefixed[instruction](cpu, instruction);
    }
}