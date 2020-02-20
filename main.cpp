#include <iostream>
#include <inc/core.h>
#include <inc/memory_pool.h>
#include <inc/decode.h>
#include <inc/asm_math_interface.h>

using namespace std;

int main(int argc, char* argv[]) {

    // for the assembly portion to work properly with the rest of the 
    // system, certain alignment requirements must be met:
    static_assert(sizeof(results_t) == 8,          "sizeof results_t incorrect (should be 8)");
    static_assert(offsetof(results_t, flags) == 0, "alignment of .flags in results_t incorrect (should be 0)");
    static_assert(offsetof(results_t,   u32) == 4, "alignment of .u32 in results_t incorrect (should be 4)");
    static_assert(offsetof(results_t,   i32) == 4, "alignment of .i32 in results_t incorrect (should be 4)");

    //armv7_m3 armcpu;
    //armcpu.set_current_mode(armv7_m3::mode_16);

    results_t results;

    cout << "\n\n";
    cout << "0x7fffffff + 0x01\n";
    cout << gp_operation(&results, 0x7fffffff, 0x01, 0, x86_asm_ADD) << endl;
    print_flag_results(results.flags);

    cout << "\n\n";
    cout << "0x7fffffff - 0x01\n";
    cout << gp_operation(&results, 0x7fffffff, 0x01, 0, x86_asm_SUB) << endl;
    print_flag_results(results.flags);
    
    cout << "\n\n";
    cout << "ADC 0x7ffffffe + 0x01 + C=0\n";
    cout << gp_operation(&results, 0x7ffffffe, 0x01, 0, x86_asm_ADC) << endl;
    print_flag_results(results.flags);
    cout << "R = " << results.i32 << endl;

    cout << "\n\n";
    cout << "ADC 0x7ffffffe + 0x01 + C=1\n";
    cout << gp_operation(&results, 0x7ffffffe, 0x01, 1, x86_asm_ADC) << endl;
    print_flag_results(results.flags);
    cout << "R = " << results.i32 << endl;

    cout << "\n\n";
    cout << "ADC 0xffffffff + 0x01 + C=0\n";
    cout << gp_operation(&results, 0xffffffff, 0x01, 0, x86_asm_ADC) << endl;
    print_flag_results(results.flags);
    cout << "R = " << results.i32 << endl;

    cout << "\n\n";
    cout << "ADC 0xfffffffe + 0x01 + C=1\n";
    cout << gp_operation(&results, 0xfffffffe, 0x01, 1, x86_asm_ADC) << endl;
    print_flag_results(results.flags);
    cout << "R = " << results.i32 << endl;

    cout << "\n\n";
    cout << "ADC 0xffffffff + 0x00 + C=1\n";
    cout << gp_operation(&results, 0xffffffff, 0x00, 1, x86_asm_ADC) << endl;
    print_flag_results(results.flags);
    cout << "R = " << results.i32 << endl;

    //cout << gp_operation(&results, 0, 0, 0, x86_asm_ADC) << endl;
    //cout << gp_operation(&results, 0, 0, 0, x86_asm_SBB) << endl;

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