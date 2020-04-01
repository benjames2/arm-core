OBJ=./obj
SRC=./src
INC=./inc

FLAGS=-std=c++11 -no-pie -march=native -O3 -I.

# defined in C++
CPPOBJ= \
 ${OBJ}/core.o \
 ${OBJ}/decode_16.o \
 ${OBJ}/decode_32.o \
 ${OBJ}/decode_structure.o \
 ${OBJ}/memory_pool.o \
 ${OBJ}/fetch.o \
 ${OBJ}/decode.o

# defined in x86_64 Assembly
ASMOBJ= \
 ${OBJ}/byte_swap.o \
 ${OBJ}/math.o

ALLOBJ=${CPPOBJ} ${ASMOBJ}

all: main

clean:
	rm ${OBJ}/*

# final executable
main: ${ALLOBJ} main.h main.cpp
	g++ -o main ${FLAGS} main.cpp ${ALLOBJ}

# build all .cpp files
${CPPOBJ}: ${OBJ}/%.o: ${SRC}/%.cpp ${INC}/%.h
	g++ -c -o $@ $< ${FLAGS}

# build all .asm files
${ASMOBJ}: ${OBJ}/%.o: ${SRC}/%.asm
	yasm -f elf64 $< -o $@
