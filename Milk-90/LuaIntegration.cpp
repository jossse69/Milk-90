// LuaIntegration.cpp
#include "LuaIntegration.h"
#include <iostream>
#include <lualib.h>
#include <lua.hpp>


LuaIntegration::LuaIntegration() {
    L = luaL_newstate();
    luaL_openlibs(L); // Open Lua libraries
}

LuaIntegration::~LuaIntegration() {
    if (L) {
        lua_close(L);
    }
}

void LuaIntegration::ExecuteScript(const char* filename) {
    if (luaL_dofile(L, filename) != LUA_OK) {
        std::cerr << "Error executing Lua script: " << lua_tostring(L, -1) << std::endl;
    }
}

lua_State* LuaIntegration::GetLuaState() const {
    return L;
}
