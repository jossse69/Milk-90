// RendererBindings.cpp
#include "RendererBindings.h"
#include <lauxlib.h>
#include "SpriteSheet.h"
#include <codecvt>
#include <iostream>

Renderer* RendererBindings::renderer = nullptr;
SpriteSheet* RendererBindings::spriteSheet = nullptr;
SpriteSheet* RendererBindings::fontSheet = nullptr;

const wchar_t* const CP_WCHAR_LOOKUP_TABLE =
L"\0☺☻♥♦♣♠•◘○◙♂♀♀♪♫☼►◄↕‼¶§▬↨↑↓→←∟↔▲▼"
L" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~⌂Çüéâäàåçêëèïî"
L"ìÄÅÉæÆôöòûùÿÖÜ¢£¥₧ƒáíóúñÑªº¿⌐¬½¼¡«»░▒"
L"▓│┤╡╢╖╕╣║╗╝╜╛┐└┴┬├─┼╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓"
L"╫╪┘┌█▄▌▐▀αßΓπΣσµτΦΘΩδ∞φε∩≡±≥≤⌠⌡÷≈°∙·√ⁿ²■ ";

std::vector<unsigned char> ConvertUTF8ToCP437(const std::string& utf8Str) {
    // Convert UTF-8 string to wide string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideStr = converter.from_bytes(utf8Str);

    std::vector<unsigned char> cp437Indexes;

    for (wchar_t wc : wideStr) {
        // Directly map ASCII characters to CP437 indexes
        const wchar_t* found = std::wcschr(CP_WCHAR_LOOKUP_TABLE, wc);
        if (found != nullptr) {
            auto index = static_cast<unsigned char>(found - CP_WCHAR_LOOKUP_TABLE);
            cp437Indexes.push_back(index);
            }
        else {
            // Character not found in lookup table, use 0 (null) as placeholder
            cp437Indexes.push_back(0);
        }
    }

    return cp437Indexes;
}

void RendererBindings::Bind(lua_State* L, Renderer* renderer, SpriteSheet* sheet, SpriteSheet* font) {
    RendererBindings::renderer = renderer;
	RendererBindings::spriteSheet = sheet;
	RendererBindings::fontSheet = font;

    lua_register(L, "clearScreen", ClearScreen);
    lua_register(L, "drawPixel", DrawPixel);
    lua_register(L, "drawLine", DrawLine);
    lua_register(L, "drawRect", DrawRect);
    lua_register(L, "drawCircle", DrawCircle);
	lua_register(L, "drawSprite", DrawSprite);
    lua_register(L, "drawText", DrawText);
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

int RendererBindings::DrawSprite(lua_State* L) {
    int x = luaL_checknumber(L, 1);
    int y = luaL_checknumber(L, 2);
    int spriteIndex = luaL_checknumber(L, 3);
    bool flipHorizontally = lua_isboolean(L, 4) ? lua_toboolean(L, 4) : false;
    bool flipVertically = lua_isboolean(L, 5) ? lua_toboolean(L, 5) : false;

    int spriteX = (spriteIndex % 32) * 8; // Column * sprite width
    int spriteY = (spriteIndex / 32) * 8; // Row * sprite height

    Sprite sprite = spriteSheet->CreateSprite(spriteX, spriteY, 8, 8);
    renderer->DrawSprite(x, y, sprite, flipHorizontally, flipVertically);
    return 0;
}

int RendererBindings::DrawText(lua_State* L) {
    int x = luaL_checknumber(L, 1);
    int y = luaL_checknumber(L, 2);
    const char* text = luaL_checkstring(L, 3);
    // Convert UTF-8 text to CP437 indices
    std::vector<unsigned char> textIndices = ConvertUTF8ToCP437(std::string(text));

    // Starting position for drawing text
    int cursorX = x;
    int cursorY = y;

    // Loop through each character in the text
    for (unsigned char index : textIndices) {
        // Assuming each character sprite is 8x8 pixels
        int spriteX = (index % 32) * 8; // Calculate sprite's X position in the font sheet
        int spriteY = (index / 32) * 8; // Calculate sprite's Y position in the font sheet

        // Draw the character sprite from the font sheet
        if (fontSheet) { // Ensure the fontSheet is valid
            Sprite charSprite = fontSheet->CreateSprite(spriteX, spriteY, 8, 8);
            renderer->DrawSprite(cursorX, cursorY, charSprite, false, false);
        }
        else
        {
			std::cout << "error drawing character " << index << std::endl;
        }

        // Move the cursor for the next character
        cursorX += 8; // Assuming fixed width characters for simplicity
    }

    return 0;
}
