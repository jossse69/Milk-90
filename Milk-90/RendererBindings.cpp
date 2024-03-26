// RendererBindings.cpp
#include "RendererBindings.h"
#include <lauxlib.h>

Renderer* RendererBindings::renderer = nullptr;

void RendererBindings::Bind(lua_State* L, Renderer* renderer) {
    RendererBindings::renderer = renderer;

    lua_register(L, "clearScreen", ClearScreen);
    lua_register(L, "drawPixel", DrawPixel);
    lua_register(L, "drawLine", DrawLine);

}

// New binding for DrawPixel
int RendererBindings::DrawPixel(lua_State* L) {
    int x = luaL_checknumber(L, 1);
    int y = luaL_checknumber(L, 2);
    int colorIndex = luaL_checknumber(L, 3);
    renderer->DrawPixel(x, y, colorIndex);
    return 0;
}

// New binding for DrawLine
int RendererBindings::DrawLine(lua_State* L) {
    int x1 = luaL_checknumber(L, 1);
    int y1 = luaL_checknumber(L, 2);
    int x2 = luaL_checknumber(L, 3);
    int y2 = luaL_checknumber(L, 4);
    int colorIndex = luaL_checknumber(L, 5);
    renderer->DrawLine(x1, y1, x2, y2, colorIndex);
    return 0;
}
int RendererBindings::ClearScreen(lua_State* L) {
    int colorIndex = luaL_checknumber(L, 1);
    // Assume ClearScreenWithColor is a method of Renderer that clears the screen with a specific palette color
    renderer->ClearScreenWithColor(colorIndex);
    return 0; // Number of return values
}
