#pragma once

#include <string>

// 16-bit decode testing
uint32_t getFormat(int format);
void test_decode_fns(std::string filename);

// 32-bit decode testing
void printBaseFormat(uint32_t number);
void test_32b_decode(std::string filename);
uint32_t get32bformat(std::string format);
