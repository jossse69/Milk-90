// InputManager.cpp
#include "InputManager.h"
#include <iostream>

InputManager::InputManager() {}

InputManager::~InputManager() {}

void InputManager::Update() {
    // Copy current key states to previous before handling new events
    CopyKeyMap();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            keyMap[event.key.keysym.scancode] = true;
        }
        else if (event.type == SDL_KEYUP) {
            keyMap[event.key.keysym.scancode] = false;
        }
        else if (event.type == SDL_QUIT) {
                quit = true;
        }
    }
}

bool InputManager::IsKeyPressed(SDL_Scancode key) {
    // Key is pressed this frame but was not pressed the previous frame
    return keyMap[key] && !previousKeyMap[key];
}

bool InputManager::IsKeyReleased(SDL_Scancode key) {
    // Key was pressed previous frame but is not pressed this frame
    return !keyMap[key] && previousKeyMap[key];
}

bool InputManager::IsKeyDown(SDL_Scancode key) {
    // Key is currently pressed
    return keyMap[key];
}

void InputManager::CopyKeyMap() {
    previousKeyMap = keyMap;
}
