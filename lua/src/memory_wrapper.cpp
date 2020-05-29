#include <lua5.3/lua.hpp>

// arm-core library
#include <inc/memory_pool.h>

extern "C" { // Lua is strictly a C library and does not contain this by default


static int new_obj(lua_State* L) {

    

}

static int delete_obj(lua_State* L) {
    memory_t* mem = (memory_t*)lua_touserdata(L, 1);
    mem.~memory_t();

    // Lua handles memory management, but we need to 'destruct' the internals.
}

}
