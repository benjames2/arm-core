# MAKEFILE FOR ARM CORE APPLICATION

gxx= g++
src= ./lib/src
obj= ./lib/obj
inc= ./lib/inc
stdopts= -std=c++11 -O3 -march=native -I${inc}

linkstr=
linkstr+= ${obj}/memory_pool.o
linkstr+= ${obj}/byte_swap.o

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
