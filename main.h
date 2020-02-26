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

