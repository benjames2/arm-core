#!/bin/bash

INC="../../"
OBJ="../../obj"

g++ core_wrapper.cpp -I${INC} ${OBJ}/core.o -o ./core_wrapper.so -std=c++11 -shared -fPIC -llua5.3

g++ memory_wrapper.cpp -I${INC} ${OBJ}/memory_pool.o ${OBJ}/byte_swap.o \
    -o ./memory_wrapper.so -std=c++11 -shared -fPIC -llua5.3

g++ execution_loop_wrapper.cpp -I${INC} ${OBJ}/fetch.o ${OBJ}/decode.o ${OBJ}/execute.o \
    -o ./execution_loop_wrapper.so -std=c++11 -shared -fPIC -llua5.3
