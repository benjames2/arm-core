# MAKEFILE FOR ARM CORE APPLICATION

gxx= g++
src= ./lib/src
obj= ./lib/obj
inc= ./lib/inc
stdopts= -std=c++11 -O3 -march=native -I${inc}

linkstr=
linkstr+= ${obj}/memory_pool.o
linkstr+= ${obj}/byte_swap.o
linkstr+= ${obj}/arm_cpu.o
linkstr+= ${obj}/instructions.o
linkstr+= ${obj}/instruction_utility.o

all: ${linkstr} main

clean:
	rm ${obj}/*

main: main.o ${linkstr}
	${gxx} -o main main.o ${linkstr} ${stdopts}

main.o: main.cpp
	${gxx} -c -o main.o main.cpp ${stdopts}

${obj}/memory_pool.o: ${src}/memory_pool.cpp ${inc}/memory_pool.h ${obj}/byte_swap.o
	${gxx} -c -o ${obj}/memory_pool.o ${src}/memory_pool.cpp ${obj}/byte_swap.o ${stdopts}

${obj}/byte_swap.o: ${src}/byte_swap.asm
	yasm -f elf64 ${src}/byte_swap.asm -o ${obj}/byte_swap.o

${obj}/arm_cpu.o: ${src}/arm_cpu.cpp
	${gxx} -c -o ${obj}/arm_cpu.o ${src}/arm_cpu.cpp ${stdopts}

${obj}/instruction_utility.o: ${src}/instruction_utility.asm
	yasm -f elf64 ${src}/instruction_utility.asm -o ${obj}/instruction_utility.o

${obj}/instructions.o: ${src}/instructions.cpp ${inc}/instructions.h ${obj}/instruction_utility.o
	${gxx} -c -o ${obj}/instructions.o ${src}/instructions.cpp ${obj}/memory_pool.o ${obj}/instruction_utility.o ${stdopts}
