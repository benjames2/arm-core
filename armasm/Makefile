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

# list of build folders. expects a simple file organizations
tmpprogs= \
 fullthumb16 \
 helloworld

progs=$(addsuffix /main.bin,${tmpprogs})

all: ${progs}

# build all available asm programs and generate bin files
${progs}: %/main.bin: %/main.s
	${AS} -mlittle-endian -march=armv7 -o $*/main.o $<
	${LD} -o $*/main.elf $*/main.o
	${OBJCOPY} -S -O binary $*/main.elf $@

clean:
	rm ./*/*.o
	rm ./*/*.elf
	rm ./*/*.bin
