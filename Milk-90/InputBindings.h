// InputBindings.h
#ifndef INPUTBINDINGS_H
#define INPUTBINDINGS_H

#include "InputManager.h"
#include <lua.hpp>
#include <string>

class InputBindings {
public:
    static void Bind(lua_State* L, InputManager* inputManager);

private:
    static InputManager* inputManager;

    // Lua-callable function to check if a key was pressed
    static int IsPressed(lua_State* L);

    // Lua-callable function to check if a key was released
    static int IsReleased(lua_State* L);

    // Lua-callable function to check if a key is down
    static int IsDown(lua_State* L);
};

#endif // INPUTBINDINGS_H
