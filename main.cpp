#include <iostream>
#include <fstream>
#include <memory_pool.h>
#include <assert.h>
#include <arm_cpu.h>

using namespace std;

int main(void) {

    arm_cpu cpu;
    cpu.set_mode(arm_mode_THUMB);

    memory_t mem(memory_t::little_endian);

    // 0x00000224    2c 48    001 01 100 01001000   - cmp r4, #0x48
    // 0x00000226    00 6a    000 00 00001 101 010  - lsl r2, r5, #0x01
    // 0x00000228    70 40    011 1 0 00001 000 000 - str r0, [r0, #0x01]
    // 0x0000022A    40 f0    010 000 0011 110 000  - lsr r0, r6
    // 0x0000022C    2a 49    001 01 010 01001001   - cmp r2, #0x49
    // 0x0000022E    08 62    000 01 00001 100 010  - lsr r2, r4, #0x01
    // 0x00000230    08 46    000 01 00001 000 110  - lsr r6, r0, #0x01
    // 0x00000232    40 6b    010 000 0001 101 011  - eor r3, r5
    // 0x00000234    70 40 20 F0   01110000 01000000 00100000 11110000 
    return 0;
}
