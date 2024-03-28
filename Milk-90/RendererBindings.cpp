// RendererBindings.cpp
#include "RendererBindings.h"
#include <lauxlib.h>
#include "SpriteSheet.h"

Renderer* RendererBindings::renderer = nullptr;
SpriteSheet* RendererBindings::spriteSheet = nullptr;

void RendererBindings::Bind(lua_State* L, Renderer* renderer) {
    RendererBindings::renderer = renderer;

    lua_register(L, "clearScreen", ClearScreen);
    lua_register(L, "drawPixel", DrawPixel);
    lua_register(L, "drawLine", DrawLine);
    lua_register(L, "drawRect", DrawRect);
    lua_register(L, "drawCircle", DrawCircle);
	lua_register(L, "drawSprite", DrawSprite);
}


int RendererBindings::DrawPixel(lua_State* L) {
    int x = luaL_checknumber(L, 1);
    int y = luaL_checknumber(L, 2);
    int colorIndex = luaL_checknumber(L, 3);
    renderer->DrawPixel(x, y, colorIndex);
    return 0;
}


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

int RendererBindings::DrawRect(lua_State* L) {
    int x = luaL_checknumber(L, 1);
    int y = luaL_checknumber(L, 2);
    int width = luaL_checknumber(L, 3);
    int height = luaL_checknumber(L, 4);
    int colorIndex = luaL_checknumber(L, 5);
    bool filled = lua_isnone(L, 6) ? true : lua_toboolean(L, 6); // Default to filled
    renderer->DrawRect(x, y, width, height, colorIndex, filled);
    return 0; // Number of return values
}

int RendererBindings::DrawCircle(lua_State* L) {
    int centerX = luaL_checknumber(L, 1);
    int centerY = luaL_checknumber(L, 2);
    int radius = luaL_checknumber(L, 3);
    int colorIndex = luaL_checknumber(L, 4);
    bool filled = lua_isnone(L, 5) ? true : lua_toboolean(L, 5); // Default to filled
    renderer->DrawCircle(centerX, centerY, radius, colorIndex, filled);
    return 0; // Number of return values
}

int RendererBindings::DrawSprite(lua_State* L)
{
    int x = luaL_checknumber(L, 1);
    int y = luaL_checknumber(L, 2);
    int spriteIndex = luaL_checknumber(L, 3);

    int spriteX = (spriteIndex % 32) * 8; // Column * sprite width
    int spriteY = (spriteIndex / 32) * 8; // Row * sprite height

    Sprite sprite = spriteSheet->CreateSprite(spriteX, spriteY, 8, 8);
    renderer->DrawSprite(x, y, sprite); // Assuming renderer is accessible and has a DrawSprite method
	return 0; 
}

