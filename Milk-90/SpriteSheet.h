#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include <string>
#include <SDL.h>
#include <vector>
#include "Sprite.h"

class SpriteSheet {
public:
    SpriteSheet();
    bool LoadFromFile(const std::string& filePath, SDL_Renderer* renderer);
    Sprite CreateSprite(int x, int y, int width, int height);

private:
    std::vector<int> pixels; // Color index for each pixel
    int sheetWidth, sheetHeight;
};

#endif // SPRITE_SHEET_H