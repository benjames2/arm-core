#include <iostream>
#include <inc/core.h>
#include <inc/memory_pool.h>
#include <inc/decode.h>
#include <inc/asm_math_interface.h>

#include "main.h"

using namespace std;

int main(int argc, char* argv[]) {

    // for the assembly portion to work properly with the rest of the 
    // system, certain alignment requirements must be met:
    static_assert(sizeof(results_t) == 8,          "sizeof results_t incorrect (should be 8)");
    static_assert(offsetof(results_t, flags) == 0, "alignment of .flags in results_t incorrect (should be 0)");
    static_assert(offsetof(results_t, u32) == 4,   "alignment of .u32 in results_t incorrect (should be 4)");
    static_assert(offsetof(results_t, i32) == 4,   "alignment of .i32 in results_t incorrect (should be 4)");

    //armv7_m3 armcpu;
    //armcpu.set_current_mode(armv7_m3::mode_16);

    test_decode_fns("testfile.branch.txt");
    test_decode_fns("testfile.bottom.txt");
    test_decode_fns("testfile.txt");

    return 0;
}

void print_flag_results(int flags) {

    cout << "    " << (flags & 0x0001 ? "1 : CY(Carry)"           : "0 : NC(No Carry)") << endl;
    cout << "    " << (flags & 0x0004 ? "1 : PE(Parity Even)"     : "0 : PO(Parity Odd)") << endl;
    cout << "    " << (flags & 0x0010 ? "1 : AC(Auxiliary Carry)" : "0 : NA(No Auxiliary Carry)") << endl;
    cout << "    " << (flags & 0x0040 ? "1 : ZR(Zero)"            : "0 : NZ(Not Zero)") << endl;
    cout << "    " << (flags & 0x0080 ? "1 : NG(Negative)"        : "0 : PL(Positive)") << endl;
    cout << "    " << (flags & 0x0800 ? "1 : OV(Overflow)"        : "0 : NV(Not Overflow)") << endl;

}