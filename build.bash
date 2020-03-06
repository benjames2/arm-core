#!/bin/bash

# pretty standard build flags. could prolly do without the -O3 flag for this 
# project, it may do some wonky things with our data
flags="-std=c++11 -march=native -no-pie -O3 -I./"

# list of object code files that need to be included in the final executable
#   obj/math.o is compiled from assembly to avoid any undefined behavior in C/C++
#       it also returns x86 flags so we have accurate ARM cpu emulation
#   obj/byte_swap.o is compiled from assembly to flex on those who dont know assembly
objs="\
    obj/memory_pool.o \
    obj/core.o \
    obj/byte_swap.o \
    obj/decode.o \
    obj/math.o" 

# this is a list of base names for various files needed to 
# build the project. each entry expands out into <name>.cpp, 
# and <name>.o for the build commands that need them
srcs=( \
    "core" \
    "memory_pool" \
    "decode" )

# if the obj/ directory doesnt exist, create it
mkdir -p obj/

# iterate through and build all object code files needed. at some point, this whole 
# thing will become a Makefile because thats basically what this is becoming anyway
for fname in "${srcs[@]}"; do
    g++ -c -o obj/${fname}.o src/${fname}.cpp ${flags}
done

# the assembly file that i stupidly made an integral part of the memory_pool library.
# unfortunately, this also semi-permanently ties this project to x86_64 architecture
yasm -f elf64 src/byte_swap.asm -o obj/byte_swap.o

# build the math assembly routines
yasm -f elf64 src/math.asm -o obj/math.o

# final executable
g++ -o main ${flags} main.cpp ${objs}


