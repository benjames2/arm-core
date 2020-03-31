OBJ=./obj
SRC=./src
INC=./inc

FLAGS=-std=c++11 -no-pie -march=native -O3 -I.

ALLOBJ= \
 ${OBJ}/byte_swap.o \
 ${OBJ}/core.o \
 ${OBJ}/decode_16.o \
 ${OBJ}/decode_32.o \
 ${OBJ}/decode_structure.o \
 ${OBJ}/math.o \
 ${OBJ}/memory_pool.o

all: main

clean:
	rm ${OBJ}/*

main: ${ALLOBJ}
	g++ -o main ${FLAGS} main.cpp ${ALLOBJ}

${OBJ}/byte_swap.o: ${SRC}/byte_swap.asm
	yasm -f elf64 src/byte_swap.asm -o obj/byte_swap.o

${OBJ}/core.o: ${SRC}/core.cpp
	g++ -c -o $@ ${SRC}/core.cpp ${FLAGS}

${OBJ}/decode_16.o: ${SRC}/decode_16.cpp
	g++ -c -o $@ ${SRC}/decode_16.cpp ${FLAGS}

${OBJ}/decode_32.o: ${SRC}/decode_32.cpp
	g++ -c -o $@ ${SRC}/decode_32.cpp ${FLAGS}

${OBJ}/decode_structure.o: ${SRC}/decode_structure.cpp
	g++ -c -o $@ ${SRC}/decode_structure.cpp ${FLAGS}

${OBJ}/math.o: ${SRC}/math.asm
	yasm -f elf64 ${SRC}/math.asm -o ${OBJ}/math.o

${OBJ}/memory_pool.o: ${SRC}/memory_pool.cpp
	g++ -c -o $@ ${SRC}/memory_pool.cpp ${FLAGS}
