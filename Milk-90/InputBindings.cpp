// InputBindings.cpp
#include "InputBindings.h"

InputManager* InputBindings::inputManager = nullptr;


SDL_Scancode GetScancodeFromString(const std::string& key) {
    if (key == "a") return SDL_SCANCODE_Z;
    if (key == "b") return SDL_SCANCODE_X;
    if (key == "left") return SDL_SCANCODE_LEFT;
    if (key == "right") return SDL_SCANCODE_RIGHT;
    if (key == "up") return SDL_SCANCODE_UP;
    if (key == "down") return SDL_SCANCODE_DOWN;
    if (key == "select") return SDL_SCANCODE_A;
    if (key == "back") return SDL_SCANCODE_S;
    return SDL_SCANCODE_UNKNOWN;
}


void InputBindings::Bind(lua_State* L, InputManager* manager) {
    inputManager = manager;

    lua_register(L, "isPressed", IsPressed);
    lua_register(L, "isReleased", IsReleased);
    lua_register(L, "isDown", IsDown);
}

int InputBindings::IsPressed(lua_State* L) {
    std::string key = luaL_checkstring(L, 1);
    SDL_Scancode scancode = GetScancodeFromString(key);
    lua_pushboolean(L, inputManager->IsKeyPressed(scancode));
    return 1; // Number of return values
}

int InputBindings::IsReleased(lua_State* L) {
    std::string key = luaL_checkstring(L, 1);
    SDL_Scancode scancode = GetScancodeFromString(key);
    lua_pushboolean(L, inputManager->IsKeyReleased(scancode));
    return 1; // Number of return values
}

int InputBindings::IsDown(lua_State* L) {
    std::string key = luaL_checkstring(L, 1);
    SDL_Scancode scancode = GetScancodeFromString(key);
    lua_pushboolean(L, inputManager->IsKeyDown(scancode));
    return 1; // Number of return values
}
