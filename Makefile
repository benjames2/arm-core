OBJ=./obj
SRC=./src
INC=./inc

FLAGS=-std=c++11 -no-pie -march=native -O3 -I.

ALLOBJ= \
 ${OBJ}/core.o \
 ${OBJ}/decode_16.o \
 ${OBJ}/decode_32.o \
 ${OBJ}/decode_structure.o \
 ${OBJ}/memory_pool.o \
 ${OBJ}/fetch.o

ASMOBJ= \
 ${OBJ}/byte_swap.o \
 ${OBJ}/math.o

all: main

clean:
	rm ${OBJ}/*

# final executable
main: ${ALLOBJ} ${ASMOBJ} main.h main.cpp
	g++ -o main ${FLAGS} main.cpp ${ALLOBJ} ${ASMOBJ}

# build all .cpp files
${ALLOBJ}: ${OBJ}/%.o: ${SRC}/%.cpp
	g++ -c -o $@ $< ${FLAGS}

# build all .asm files
${ASMOBJ}: ${OBJ}/%.o: ${SRC}/%.asm
	yasm -f elf64 $< -o $@
