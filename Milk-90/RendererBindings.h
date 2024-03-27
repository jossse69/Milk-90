// RendererBindings.h
#ifndef RENDERER_BINDINGS_H
#define RENDERER_BINDINGS_H

#include "Renderer.h"
#include <lua.hpp>

class RendererBindings {
public:
    static void Bind(lua_State* L, Renderer* renderer);


private:
    static Renderer* renderer;


    // Lua-callable function
    static int ClearScreen(lua_State* L);
    static int DrawPixel(lua_State* L);
    static int DrawLine(lua_State* L);
	static int DrawRect(lua_State* L);
	static int DrawCircle(lua_State* L);

};

#endif // RENDERER_BINDINGS_H
