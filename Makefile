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
 ${OBJ}/decode.o \
 ${OBJ}/execute.o \
 ${OBJ}/import.o \
 ${OBJ}/test.o \
 ${OBJ}/stack_operations.o \
 ${OBJ}/exceptions.o

# defined in x86_64 Assembly
ASMOBJ= ${OBJ}/byte_swap.o
ASMMATHOBJ= ${OBJ}/math.o # <-- this one is awkward

ALLOBJ=${CPPOBJ} ${ASMOBJ} ${ASMMATHOBJ}
ALLOBJLUA=${CPPOBJ}

all: main

# the lua wrapper requires all of the object files but not the final executable
lua: luaprep ${ALLOBJ} ${INC}/static_asserts.h

luaprep:
	$(eval FLAGS += -fPIC -shared)

# count the number of lines of code in the project
count:
	cloc src/ inc/ ./

clean:
	rm ${OBJ}/*

# final executable
main: ${ALLOBJ} main.h ${INC}/static_asserts.h main.cpp
	g++ -o main ${FLAGS} main.cpp ${ALLOBJ}

# build all .cpp files
${CPPOBJ}: ${OBJ}/%.o: ${SRC}/%.cpp ${INC}/%.h
	g++ -c -o $@ $< ${FLAGS}

# build all .asm files (except for the special asm math stuff)
${ASMOBJ}: ${OBJ}/%.o: ${SRC}/%.asm
	yasm -f elf64 $< -o $@

# asm math stuff has a weird dependency chain. this is the most 
# straightforward fix. it doesnt fit nicely with the other stuff 
# but that is how it be sometimes
#
# 1.) generate two temporary object files
# 2.) combine them into a single object file with ld
# 3.) remove temp object files
#
${ASMMATHOBJ}: ${SRC}/math.asm ${SRC}/asm_math_interface.cpp
	yasm -f elf64 ${SRC}/math.asm -o ${OBJ}/tmp_asm_math.o
	g++ -c -o ${OBJ}/tmp_cpp_math.o ${SRC}/asm_math_interface.cpp ${FLAGS}
	ld -relocatable ${OBJ}/tmp_cpp_math.o ${OBJ}/tmp_asm_math.o -o $@
	rm ${OBJ}/tmp_cpp_math.o
	rm ${OBJ}/tmp_asm_math.o
