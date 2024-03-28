#ifndef SPRITE_H
#define SPRITE_H

#include <vector>
class Sprite {
public:
    int width, height;
    std::vector<int> pixels; // Using -1 for transparency

    Sprite(int w, int h, std::initializer_list<int> p) : width(w), height(h), pixels(p) {}
    Sprite(int w, int h, const std::vector<int>& p) : width(w), height(h), pixels(p) {}

    int GetPixel(int x, int y) const {
        if (x < 0 || y < 0 || x >= width || y >= height) return -1; // Out of bounds or transparent
        return pixels[y * width + x];
    }
};

#endif // SPRITE_H