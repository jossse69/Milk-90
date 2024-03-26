// LuaIntegration.h
#ifndef LUA_INTEGRATION_H
#define LUA_INTEGRATION_H

#include <lua.hpp>

class LuaIntegration {
public:
    LuaIntegration();
    ~LuaIntegration();

    void ExecuteScript(const char* filename);
    lua_State* GetLuaState() const;

private:
    lua_State* L;
};

#endif // LUA_INTEGRATION_H
