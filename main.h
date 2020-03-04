#pragma once

#define SHL(n) (1 << n)

#define BASE_FORMAT_1  (0) // no bits on by default
#define BASE_FORMAT_2  (SHL(12) | SHL(11) | 0)
#define BASE_FORMAT_3  (SHL(13) | 0)
#define BASE_FORMAT_4  (SHL(14) | 0)
#define BASE_FORMAT_5  (SHL(14) | SHL(10) | 0)
#define BASE_FORMAT_6  (SHL(14) | SHL(11) | 0)
#define BASE_FORMAT_7  (SHL(14) | SHL(12) | 0)
#define BASE_FORMAT_8  (SHL(14) | SHL(12) | SHL(9) | 0)
#define BASE_FORMAT_9  (SHL(14) | SHL(13) | 0)
#define BASE_FORMAT_10 (SHL(15) | 0)
#define BASE_FORMAT_11 (SHL(15) | SHL(12) | 0)
#define BASE_FORMAT_12 (SHL(15) | SHL(13) | 0)
#define BASE_FORMAT_13 (SHL(15) | SHL(13) | SHL(12) | 0)
#define BASE_FORMAT_14 (SHL(15) | SHL(13) | SHL(12) | SHL(10) | 0)
#define BASE_FORMAT_15 (SHL(15) | SHL(14) | 0)
#define BASE_FORMAT_16 (SHL(15) | SHL(14) | SHL(12) | 0)
#define BASE_FORMAT_17 (SHL(15) | SHL(14) | SHL(12) | SHL(11) | SHL(10) | SHL(9) | SHL(8) | 0)
#define BASE_FORMAT_18 (SHL(15) | SHL(14) | SHL(13) | 0)
#define BASE_FORMAT_19 (SHL(15) | SHL(14) | SHL(13) | SHL(12) | 0)

#include <iostream>
#include <inc/decode.h>

void test_decode_fns(void) {

    {
        const uint32_t in = BASE_FORMAT_4 | (0x05 << 6) | (0x03 << 3) | (0x01 << 0);
        std::cout << "E: ADC r1, r3\n"; // expected
        auto dec_str = decode_instruction(0x00, in).str();
        std::cout << "G: " << dec_str << std::endl; // given
    }

    {
        
    }

}

#undef SHL
