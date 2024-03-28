// Renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <vector> // Include for std::vector
#include "Sprite.h"

class Renderer {
public:
    static int const PALETTE_SIZE;
    static constexpr SDL_Color PALETTE[] = {
    {0x28, 0x28, 0x2e, 0xFF}, // #28282e
    {0x6c, 0x56, 0x71, 0xFF}, // #6c5671
    {0xd9, 0xc8, 0xbf, 0xFF}, // #d9c8bf
    {0xf9, 0x82, 0x84, 0xFF}, // #f98284
    {0xb0, 0xa9, 0xe4, 0xFF}, // #b0a9e4
    {0xac, 0xcc, 0xe4, 0xFF}, // #accce4
    {0xb3, 0xe3, 0xda, 0xFF}, // #b3e3da
    {0xfe, 0xaa, 0xe4, 0xFF}, // #feaae4
    {0x87, 0xa8, 0x89, 0xFF}, // #87a889
    {0xb0, 0xeb, 0x93, 0xFF}, // #b0eb93
    {0xe9, 0xf5, 0x9d, 0xFF}, // #e9f59d
    {0xff, 0xe6, 0xc6, 0xFF}, // #ffe6c6
    {0xde, 0xa3, 0x8b, 0xFF}, // #dea38b
    {0xff, 0xc3, 0x84, 0xFF}, // #ffc384
    {0xff, 0xf7, 0xa0, 0xFF}, // #fff7a0
    {0xff, 0xf7, 0xe4, 0xFF}  // #fff7e4
    };
    Renderer(int width, int height, const char* windowTitle, int gridWidth, int gridHeight);
    ~Renderer();

    void RenderGrid(int gridWidth, int gridHeight);
    void Present();
    bool IsInitialized() const;
    SDL_Renderer* GetSDLRenderer() const; // Getter function for SDL renderer

	void ClearScreenWithColor(unsigned char color);
    void DrawPixel(int x, int y, int colorIndex);
    void DrawLine(int x1, int y1, int x2, int y2, int colorIndex);
    void DrawRect(int x, int y, int width, int height, int colorIndex, bool filled = true);
    void DrawCircle(int centerX, int centerY, int radius, int colorIndex, bool filled = true);
    void DrawSprite(int x, int y, const Sprite& sprite);



private:
    SDL_Window* window = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;
    SDL_Texture* texture = nullptr;
    int screenWidth;
    int screenHeight;
	int gridWidth;
    int gridHeight;
    bool initialized = false;

    std::vector<unsigned char> grid;
    void CreateTexture(int width, int height);

};

#endif // RENDERER_H
