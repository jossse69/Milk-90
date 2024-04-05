// Renderer.cpp
#include "Renderer.h"
#include <iostream>
#include <vector>
#include "Sprite.h"

const int Renderer::PALETTE_SIZE = sizeof(Renderer::PALETTE) / sizeof(SDL_Color);

Renderer::Renderer(int width, int height, const char* windowTitle, int gridwidth, int gridheight) {

    window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created. SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdlRenderer) {
        std::cerr << "Renderer could not be created. SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    screenWidth = width;
    screenHeight = height;

    gridWidth = gridwidth;
	gridHeight = gridheight;

    grid = std::vector<unsigned char>(gridWidth * gridHeight, 0); // Initialize all to 0 (first color in the palette)
    
    CreateTexture(gridWidth, gridHeight); // Create a texture for pixel drawing

    initialized = true;
}

Renderer::~Renderer() {
    if (texture) SDL_DestroyTexture(texture);
    if (sdlRenderer) SDL_DestroyRenderer(sdlRenderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::CreateTexture(int width, int height) {
    // Create a texture for rendering
    texture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        std::cerr << "Could not create texture. SDL_Error: " << SDL_GetError() << std::endl;
        initialized = false;
    }
}

void Renderer::RenderGrid(int gridWidth, int gridHeight) {
    // Lock texture for manipulation
    void* pixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    // Convert grid IDs to actual colors and fill in the pixels
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            unsigned char gridValue = grid[y * gridWidth + x];
            if (gridValue < PALETTE_SIZE) {
                Uint32 color = (PALETTE[gridValue].r << 24) | (PALETTE[gridValue].g << 16) | (PALETTE[gridValue].b << 8) | PALETTE[gridValue].a;
                ((Uint32*)pixels)[y * (pitch / 4) + x] = color;
            }
        }
    }

    SDL_UnlockTexture(texture);

    // Calculate scaling factor based on window size and original resolution
    float scaleX = (float)screenWidth / gridWidth;
    float scaleY = (float)screenHeight / gridHeight;

    SDL_Rect destRect;
    destRect.x = 0;
    destRect.y = 0;
    destRect.w = gridWidth * scaleX;  // Scale width
    destRect.h = gridHeight * scaleY; // Scale height

    // Render the scaled texture to the window
    SDL_RenderCopy(sdlRenderer, texture, NULL, &destRect);
}

void Renderer::Present() {
    SDL_RenderPresent(sdlRenderer);
}

bool Renderer::IsInitialized() const {
    return initialized;
}

SDL_Renderer* Renderer::GetSDLRenderer() const {
    return sdlRenderer;
}

void Renderer::ClearScreenWithColor(unsigned char color) {

	// Clear the screen
    for (int i = 0; i < grid.size(); i++)
    {
        grid[i] = color;
    }

}

void Renderer::DrawPixel(int x, int y, int colorIndex) {
    if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) return; // Use gridWidth and gridHeight for bounds checking
    grid[y * gridWidth + x] = colorIndex; // Use gridWidth for calculating the index
}


void Renderer::DrawLine(int x1, int y1, int x2, int y2, int colorIndex) {
    // Bresenham's line algorithm
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;) { /* loop */
        DrawPixel(x1, y1, colorIndex);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void Renderer::DrawRect(int x, int y, int width, int height, int colorIndex, bool filled) {

	// Draw a rectangle, with fill or not
    if (filled) {
        for (int i = y; i < y + height; ++i) {
            for (int j = x; j < x + width; ++j) {
                DrawPixel(j, i, colorIndex); // Use existing method
            }
        }
    }
    else {
        // Draw the outline
        for (int i = x; i < x + width; ++i) {
            DrawPixel(i, y, colorIndex);
            DrawPixel(i, y + height - 1, colorIndex);
        }
        for (int i = y; i < y + height; ++i) {
            DrawPixel(x, i, colorIndex);
            DrawPixel(x + width - 1, i, colorIndex);
        }
    }
}

void Renderer::DrawCircle(int centerX, int centerY, int radius, int colorIndex, bool filled) {
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        if (filled) {
            // Draw vertical lines for a filled circle
            for (int i = centerX - y; i <= centerX + y; ++i) {
                DrawPixel(i, centerY + x, colorIndex);
                DrawPixel(i, centerY - x, colorIndex);
            }
            for (int i = centerX - x; i <= centerX + x; ++i) {
                DrawPixel(i, centerY + y, colorIndex);
                DrawPixel(i, centerY - y, colorIndex);
            }
        }
        else {
            // Draw points for an outlined circle
			DrawPixel(centerX + x, centerY + y, colorIndex);
			DrawPixel(centerX - x, centerY + y, colorIndex);
			DrawPixel(centerX + x, centerY - y, colorIndex);
			DrawPixel(centerX - x, centerY - y, colorIndex);
			DrawPixel(centerX + y, centerY + x, colorIndex);
			DrawPixel(centerX - y, centerY + x, colorIndex);
			DrawPixel(centerX + y, centerY - x, colorIndex);
			DrawPixel(centerX - y, centerY - x, colorIndex);
		}
        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void Renderer::DrawSprite(int x, int y, const Sprite& sprite, bool flipHorizontally, bool flipVertically) {
    for (int spriteY = 0; spriteY < sprite.height; ++spriteY) {
        for (int spriteX = 0; spriteX < sprite.width; ++spriteX) {
            // Determine the source pixel based on flip flags
            int srcX = flipHorizontally ? (sprite.width - 1 - spriteX) : spriteX;
            int srcY = flipVertically ? (sprite.height - 1 - spriteY) : spriteY;

            int colorIndex = sprite.GetPixel(srcX, srcY);
            if (colorIndex != -1) { // -1 is transparent
                DrawPixel(x + spriteX, y + spriteY, colorIndex);
            }
        }
    }
}
