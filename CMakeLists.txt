cmake_minimum_required(VERSION 3.17)
project(CBoy C)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

ADD_EXECUTABLE(
    CBoy
        main.c
        include/log.h
        include/default.h
        src/System/GB.c
        src/System/GB.h
        src/System/CPU/CPU.c
        src/System/CPU/CPU.h
        src/System/CPU/instructions.c
        src/System/MEM/MEM.c
        src/System/MEM/MEM.h
        src/System/PPU/PPU.c
        src/System/PPU/PPU.h
        src/System/Display/display.c
        src/System/Display/display.h
        src/System/IO/IO.c
        src/System/IO/IO.h
        src/System/PPU/sprites.h
        src/System/IO/LCD.h
        src/System/IO/joypad.c
        src/System/IO/joypad.h
        src/System/IO/interrupts.h
        src/System/IO/interrupts.c
        src/System/IO/LCD.c)

# for logging / stuff I want to use in all files
include_directories(./include)

target_link_libraries(CBoy PRIVATE ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2)

# add to turn on aggressive optimizations you can also add -O1 -O2 or -O3 to ADD_COMPILE_OPTIONS
# or -Ofast (super fast) or -Oz (small code and optimized)
# -O3 being the most aggressive
ADD_COMPILE_OPTIONS(-Wall -Werror -fsanitize=address -O2)
ADD_LINK_OPTIONS(-fsanitize=address)