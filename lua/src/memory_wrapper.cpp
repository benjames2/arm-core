#include <lua5.3/lua.hpp>
#include <sstream>
#include <inc/memory_pool.h>

extern "C" { // Lua is strictly a C library and does not contain this by default

static int total_objects = 0;

static int new_obj(lua_State* L) {

    // allocate memory that Lua will manage for us
    void* vptr = lua_newuserdata(L, sizeof(memory_t));
    double d = luaL_checknumber(L, 1);

    if(d > 0.9 && d < 1.1)
        memory_t* memptr = new(static_cast<memory_t*>(vptr)) memory_t(memory_t::big_endian);
    else if(d > -0.1 && d < 0.1)
        memory_t* memptr = new(static_cast<memory_t*>(vptr)) memory_t(memory_t::little_endian);

    // for the memory_t ptrs above, it doesnt matter if they go out of scope because Lua hasnt lost track of the memory

    total_objects++;
    return 1;
}

static int delete_obj(lua_State* L) {
    memory_t* mem = static_cast<memory_t*>(lua_touserdata(L, 1));
    mem->~memory_t();
    total_objects--;

    // Lua handles memory management, but we need to 'destruct' the internals.
}

static int load_u8(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    lua_pushnumber(L, memptr->load_u8(addr));
    return 1;
}

static int load_u16(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    lua_pushnumber(L, memptr->load_u16(addr));
    return 1;
}

static int load_u32(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    lua_pushnumber(L, memptr->load_u32(addr));
    return 1;
}

static int load_i8(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    lua_pushnumber(L, memptr->load_i8(addr));
    return 1;
}

static int load_i16(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    lua_pushnumber(L, memptr->load_i16(addr));
    return 1;
}

static int load_i32(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    lua_pushnumber(L, memptr->load_i32(addr));
    return 1;
}

static int store_u8(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    memptr->store_u8(addr, luaL_checknumber(L, 3));
    return 0;
}

static int store_u16(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    memptr->store_u16(addr, luaL_checknumber(L, 3));
    return 0;
}

static int store_u32(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    memptr->store_u32(addr, luaL_checknumber(L, 3));
    return 0;
}

static int store_i8(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    memptr->store_i8(addr, luaL_checknumber(L, 3));
    return 0;
}

static int store_i16(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    memptr->store_i16(addr, luaL_checknumber(L, 3));
    return 0;
}

static int store_i32(lua_State* L) {
    auto* memptr = (memory_t*)lua_touserdata(L, 1);
    address_t addr = (address_t)luaL_checknumber(L, 2);
    memptr->store_i32(addr, luaL_checknumber(L, 3));
    return 0;
}

static int to_string(lua_State* L) {

    auto* memptr = (memory_t*)lua_touserdata(L, 1);

    std::stringstream ss;
    ss << (*memptr);

    std::string str = ss.str();

    lua_pushlstring(L, str.c_str(), str.size());
    return 1;
}

static int get_num_objects(lua_State* L) {
    lua_pushnumber(L, total_objects);
    return 1;
}

const struct luaL_Reg regarray[] = {
    // constructor and destructor
    { "new",           new_obj     },
    { "delete",        delete_obj  },

    // memory access methods
    { "load_u8",  load_u8  },
    { "load_u16", load_u16 },
    { "load_u32", load_u32 },
    { "load_i8",  load_i8  },
    { "load_i16", load_i16 },
    { "load_i32", load_i32 },
    { "store_u8",  store_u8  },
    { "store_u16", store_u16 },
    { "store_u32", store_u32 },
    { "store_i8",  store_i8  },
    { "store_i16", store_i16 },
    { "store_i32", store_i32 },

    { "to_string", to_string },
    { "debugNumObjects",  get_num_objects },
    { NULL, NULL } // <-- last entry must be {NULL, NULL}
};

// initialization function must be luaopen_<name of .so>
int luaopen_memory_wrapper(lua_State* L) {
    // create new metatable to be used for future memory_t objects
    //luaL_newmetatable(L, "memory.metatable");

    luaL_newlib(L, regarray);
    return 1;
}

}
