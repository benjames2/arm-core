gxx= g++
src= ./lib/src
obj= ./lib/obj
inc= ./lib/inc
stdopts= -std=c++11 -O3 -march=native -I${inc}

linkstr=
linkstr+= ${obj}/memory_pool.o

all: ${linkstr} main

main: main.o ${linkstr}
	${gxx} -o main main.o ${linkstr} ${stdopts}

main.o: main.cpp
	${gxx} -c -o main.o main.cpp ${stdopts}

${obj}/memory_pool.o: ${src}/memory_pool.cpp ${inc}/memory_pool.h
	${gxx} -c -o ${obj}/memory_pool.o ${src}/memory_pool.cpp ${stdopts}
