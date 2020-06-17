#include <lua5.3/lua.hpp>
#include <sstream>

// 
#include <inc/fetch.h>
#include <inc/decode.h>
#include <inc/execute.h>

extern "C" { // Lua is strictly a C library and does not contain this by default

static int step_execute(lua_State* L) {

    auto* cpu = (armv7_m3*)lua_touserdata(L, 1);
    auto* mem = (memory_t*)lua_touserdata(L, 2);

    auto inst_data   = fetch(*mem, cpu->PC(), true);
    auto decode_data = decode(inst_data, cpu->PC());
    *cpu = execute(*cpu, *mem, decode_data);
}

const struct luaL_Reg regarray[] = {
    { "step", step_execute },
    { NULL,   NULL } // <-- last entry must be {NULL, NULL}
};

// initialization function must be luaopen_<name of .so>
int luaopen_execution_loop_wrapper(lua_State* L) {

    luaL_newlib(L, regarray);
    return 1;
}

}