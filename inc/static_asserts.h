#pragma once

// for the assembly portion to work properly with the rest of the 
// system, certain alignment requirements must be met:
static_assert(sizeof(results_t) == 8,          "sizeof results_t incorrect (should be 8)");
static_assert(offsetof(results_t, flags) == 0, "alignment of .flags in results_t incorrect (should be 0)");
static_assert(offsetof(results_t, u32) == 4,   "alignment of .u32 in results_t incorrect (should be 4)");
static_assert(offsetof(results_t, i32) == 4,   "alignment of .i32 in results_t incorrect (should be 4)");

