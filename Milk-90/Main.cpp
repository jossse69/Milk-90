#include <SDL.h>
#include "Renderer.h"
#include <iostream>
#include <vector>
#include "LuaIntegration.h"
#include "RendererBindings.h"
#include "InputBindings.h"
#include "Sprite.h"
#include "SDL_image.h"
#include "SpriteSheet.h"
int main(int argc, char* argv[]) {
    // Window dimensions (double of the original res)
    const int windowWidth = 510;
    const int windowHeight = 340;

    const int gridWidth = 255; // Same as window for simplicity
    const int gridHeight = 170; // Same as window for simplicity

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		return 1;
	}
    
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return 1;
    }
    
    // Create the renderer
    Renderer renderer(windowWidth, windowHeight, "Milk-90", gridWidth, gridHeight);

    if (!renderer.IsInitialized()) {
        std::cerr << "Renderer failed to initialize." << std::endl;
        return -1;
    }

    // Main loop flag
    bool quit = false;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;


    // Event handler
    SDL_Event e;

    InputManager inputManager;

    // load the sprite sheet of the project
    SpriteSheet* spriteSheet = new SpriteSheet();
    if (!spriteSheet->LoadFromFile("project/sprites.png", renderer.GetSDLRenderer()))
    {
        std::cout << "Sprite sheet not loaded!" << std::endl;
    }

    LuaIntegration lua;
    InputBindings::Bind(lua.GetLuaState(), &inputManager);
    RendererBindings::Bind(lua.GetLuaState(), &renderer, spriteSheet);

    Uint32 lastTime = SDL_GetTicks(), currentTime, deltaTime;




    lua.ExecuteScript("project/main.lua");


	// call the 'load' function
    lua_getglobal(lua.GetLuaState(), "load");
    if (lua_isfunction(lua.GetLuaState(), -1))
    {
        if (lua_pcall(lua.GetLuaState(), 0, 0, 0) != LUA_OK) {
            std::cerr << "Error calling load: " << lua_tostring(lua.GetLuaState(), -1) << std::endl;
        }
    }
    else {
        // Pop the non-function value from the stack
        lua_pop(lua.GetLuaState(), 1);
    }


    // While application is running
    while (!quit) {
        // Handle events
		inputManager.Update();

		// user wants to quit
		if (inputManager.quit)
			quit = true;

        currentTime = SDL_GetTicks();
        frameStart = currentTime;
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;


        // Convert deltaTime to seconds
        float deltaTimeInSeconds = deltaTime / 1000.0f;


        // Call the 'update' function with deltaTimeInSeconds as a parameter
        lua_getglobal(lua.GetLuaState(), "update");
        if (lua_isfunction(lua.GetLuaState(), -1))
        {
            lua_pushnumber(lua.GetLuaState(), deltaTimeInSeconds);
            if (lua_pcall(lua.GetLuaState(), 1, 0, 0) != LUA_OK) {
                std::cerr << "Error calling update: " << lua_tostring(lua.GetLuaState(), -1) << std::endl;
            }
        }
        else {
            // Pop the non-function value from the stack
            lua_pop(lua.GetLuaState(), 1);
        }

        // Clear screen
        SDL_RenderClear(renderer.GetSDLRenderer());


        // Call the 'draw' function if it exists
        lua_getglobal(lua.GetLuaState(), "draw");
        if (lua_isfunction(lua.GetLuaState(), -1)) {
            if (lua_pcall(lua.GetLuaState(), 0, 0, 0) != LUA_OK) {
                std::cerr << "Error calling draw: " << lua_tostring(lua.GetLuaState(), -1) << std::endl;
            }
        }
        else {
            // Pop the non-function value from the stack
            lua_pop(lua.GetLuaState(), 1);
        }


        // Render the grid to the texture and present it
        renderer.RenderGrid(gridWidth, gridHeight);
        renderer.Present();

        // Calculate how long processing the frame took
        frameTime = SDL_GetTicks() - frameStart;

        // If the frame was processed faster than our frameDelay, delay the loop to cap the FPS
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

    }

    IMG_Quit(); // Quit SDL_image

    return 0;
}
