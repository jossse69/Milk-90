// Renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <vector> // Include for std::vector

class Renderer {
public:
    Renderer(int width, int height, const char* windowTitle, int gridWidth, int gridHeight);
    ~Renderer();

    void RenderGrid(int gridWidth, int gridHeight);
    void Present();
    bool IsInitialized() const;
    SDL_Renderer* GetSDLRenderer() const; // Getter function for SDL renderer

	void ClearScreenWithColor(unsigned char color);
    void DrawPixel(int x, int y, int colorIndex);
    void DrawLine(int x1, int y1, int x2, int y2, int colorIndex);



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
