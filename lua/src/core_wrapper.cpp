#include <lua5.3/lua.hpp>

#include <sstream>

// arm-core library
#include <inc/core.h>

extern "C" { // Lua is strictly a C library and does not contain this by default

int total_objects = 0;

static int new_obj(lua_State* L) {
    
    void* armptr = lua_newuserdata(L, sizeof(armv7_m3));
    auto* newarmptr = 
        new( // placement new
            static_cast<armv7_m3*>(armptr)) armv7_m3; 

    luaL_getmetatable(L, "armcore.metatable");
    lua_setmetatable(L, -2); // set metatable for constructed userdata object

    //std::cout << "C -> constructing armv7_m3 object\n";
    total_objects++;

    return 1;
}

static int delete_obj(lua_State* L) {

    auto* armcore = (armv7_m3*)lua_touserdata(L, 1);    
    (*armcore).~armv7_m3();

    total_objects--;

    //std::cout << "C -> destructing armv7_m3 object, " << total_objects << " objects remaining...\n";

    return 0;
}

static int get_flags(lua_State* L) {
    auto* armcore = (armv7_m3*)lua_touserdata(L, 1);    

    lua_pushnumber(L, armcore->get_APSR_N());
    lua_pushnumber(L, armcore->get_APSR_Z());
    lua_pushnumber(L, armcore->get_APSR_C());
    lua_pushnumber(L, armcore->get_APSR_V());
    lua_pushnumber(L, armcore->get_APSR_Q());

    return 5;
}

static int to_string(lua_State* L) {

    auto* armcore = (armv7_m3*)lua_touserdata(L, 1);

    std::stringstream ss;
    ss << (*armcore);

    std::string str = ss.str();

    lua_pushlstring(L, str.c_str(), str.size());
    return 1;
}

static int serialize(lua_State* L) {

    auto* armcore = (armv7_m3*)lua_touserdata(L, 1);

    auto str = armcore->serialize();
    lua_pushlstring(L, str.c_str(), str.size());
    return 1;
}

static int deserialize(lua_State* L) {

    const char* cptr = luaL_checkstring(L, 1);
    auto* armcore = (armv7_m3*)lua_touserdata(L, 2);

    // deserialize into proper memory location for new cpu object
    armv7_m3::deserialize(cptr, armcore);    

    //std::cout << "C -> constructing armv7_m3 object\n";

    return 1;
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
    { "new",           new_obj     },
    { "delete",        delete_obj  },
    { "to_string",     to_string   },
    { "serialize",     serialize   },
    { "deserialize",   deserialize },
    { "get_all_flags", get_flags   },
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
