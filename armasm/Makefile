#
# makefile for assembling ARM asm programs
#

SHELL := bash

CC=arm-none-eabi-gcc
AS=arm-none-eabi-as
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
ADDR2LINE=arm-none-eabi-addr2line
AR=arm-none-eabi-ar
GDB=arm-none-eabi-gdb

# list of build folders. expects a simple file organization
tmpprogs= \
 fullthumb16 \
 helloworld

progs=$(addsuffix /main.bin,${tmpprogs})

all: ${progs}

#
# build all available asm programs and generate bin files
#
# Steps:
#    (as)      main.s   -> main.o
#    (ld)      main.o   -> main.elf
#    (objcopy) main.elf -> main.bin
#
${progs}: %/main.bin: %/main.s
	${AS} -ggdb -mlittle-endian -march=armv7 -mthumb -o $*/main.o $< 
	${LD} -T linkage.lds -o $*/main.elf $*/main.o
	${OBJCOPY} -S -O binary $*/main.elf $@

clean:
	rm ./*/*.o
	rm ./*/*.elf
	rm ./*/*.bin
