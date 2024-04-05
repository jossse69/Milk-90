
#include "SpriteSheet.h"
#include "Sprite.h"
#include "SDL_image.h"
#include <iostream>
#include "Renderer.h"


int RGBToPaletteIndex(Uint8 r, Uint8 g, Uint8 b, Uint16 a) {
    // Transparent color? Treat as -1
    if (a == 0) {
        return -1;
    }

    int closestIndex = 0;
    int smallestDifference = INT_MAX;
    for (int i = 0; i < Renderer::PALETTE_SIZE; ++i) {
        int dr = r - Renderer::PALETTE[i].r;
        int dg = g - Renderer::PALETTE[i].g;
        int db = b - Renderer::PALETTE[i].b;
        int difference = dr * dr + dg * dg + db * db;
        if (difference < smallestDifference) {
            closestIndex = i;
            smallestDifference = difference;
        }
    }
    return closestIndex;
}

SpriteSheet::SpriteSheet() {
    sheetHeight = 255;
    sheetWidth = 255;
    pixels.resize(sheetWidth * sheetHeight, 0); // Initialize pixels with zeros
}

bool SpriteSheet::LoadFromFile(const std::string& filePath, SDL_Renderer* renderer) {
    // Load the image
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load sprite sheet: " << IMG_GetError() << std::endl;
        return false;
    }

    // Ensure the surface is in a format we expect (32-bit)
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(surface); // Free the original surface
    if (!formattedSurface) {
        std::cerr << "Failed to convert sprite sheet to proper format." << std::endl;
        return false;
    }

    // Process the pixels
    Uint32* pixelsData = static_cast<Uint32*>(formattedSurface->pixels);
    this->pixels.clear(); // Clear existing pixels in the sprite sheet
    for (int i = 0; i < formattedSurface->w * formattedSurface->h; ++i) {
        Uint32 pixel = pixelsData[i];
        Uint8 r, g, b, a;
        SDL_GetRGBA(pixel, formattedSurface->format, &r, &g, &b, &a);
        int colorIndex = RGBToPaletteIndex(r, g, b, a);
        this->pixels.push_back(colorIndex);
    }

    // Set the actual width and height of the sprite sheet
    sheetWidth = formattedSurface->w;
    sheetHeight = formattedSurface->h;

    SDL_FreeSurface(formattedSurface);
    return true;
}


Sprite SpriteSheet::CreateSprite(int x, int y, int width, int height) {
    std::vector<int> spritePixels(width * height);

    for (int spriteY = 0; spriteY < height; ++spriteY) {
        for (int spriteX = 0; spriteX < width; ++spriteX) {
            int sheetIndex = (y + spriteY) * sheetWidth + (x + spriteX);
            spritePixels[spriteY * width + spriteX] = pixels[sheetIndex];
        }
    }

    return Sprite(width, height, spritePixels);
}

