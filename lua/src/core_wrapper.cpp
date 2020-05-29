#include <lua5.3/lua.hpp>

// arm-core library
#include <inc/core.h>

extern "C" { // Lua is strictly a C library and does not contain this by default

static int new_obj(lua_State* L) {
    
    void* armptr = lua_newuserdata(L, sizeof(armv7_m3));
    auto* newarmptr = 
        new( // placement new
            static_cast<armv7_m3*>(armptr)) armv7_m3; 

    luaL_getmetatable(L, "armcore.metatable");
    lua_setmetatable(L, -2);

    return 1;
}

static int delete_obj(lua_State* L) {

    //std::cout << "Deleting arm cpu object" << std::endl;

    auto* armcore = (armv7_m3*)lua_touserdata(L, 1);
    
    (*armcore).~armv7_m3();

    // Lua handles memory management, but we need to 'destruct' the internals.
    // tho in this case we dont even really need to do that. for more complex 
    // objects we def need to call destructor explicitly

    return 0;
}

static int get_register_u32(lua_State* L) {
    auto* armcore = (armv7_m3*)lua_touserdata(L, 1);
    int idx = luaL_checknumber(L, 2);
    unsigned int r = armcore->get_register_u32(idx);
    lua_pushnumber(L, r);

    return 1;
}

static int get_register_i32(lua_State* L) {
    auto* armcore = (armv7_m3*)lua_touserdata(L, 1);
    int idx = luaL_checknumber(L, 2);
    int r = armcore->get_register_u32(idx);
    lua_pushnumber(L, r);

    return 1;
}

static int set_register_i32(lua_State* L) {
    auto* armcore = (armv7_m3*)lua_touserdata(L, 1);
    int idx = luaL_checknumber(L, 2);
    int val = luaL_checknumber(L, 3);
    armcore->set_register_i32(idx, val);

    return 0;
}

static int set_register_u32(lua_State* L) {
    auto* armcore = (armv7_m3*)lua_touserdata(L, 1);
    int idx = luaL_checknumber(L, 2);
    unsigned int val = luaL_checknumber(L, 3);
    armcore->set_register_i32(idx, val);

    return 0;
}

const struct luaL_Reg regarray[] = {
    { "new",       new_obj },
    { "delete",    delete_obj },
    { "get_register_u32", get_register_u32 },
    { "get_register_i32", get_register_i32 },
    { "set_register_u32", set_register_u32 },
    { "set_register_i32", set_register_i32 },
    { NULL, NULL } // <-- last entry must be {NULL, NULL}
};

// initialization function must be luaopen_<name of .so>
int luaopen_core_wrapper(lua_State* L) {
    // create new metatable to be used for future armv7_m3 objects
    luaL_newmetatable(L, "armcore.metatable");



    luaL_newlib(L, regarray);
    return 1;
}

}
