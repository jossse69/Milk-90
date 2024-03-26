// InputManager.h
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL.h>
#include <map>

class InputManager {
public:
    InputManager();
    ~InputManager();

    void Update(); // Call this each frame to update the input states
    bool IsKeyPressed(SDL_Scancode key);
    bool IsKeyReleased(SDL_Scancode key);
    bool IsKeyDown(SDL_Scancode key);

    bool quit = false;

private:
    std::map<SDL_Scancode, bool> keyMap; // Current frame key states
    std::map<SDL_Scancode, bool> previousKeyMap; // Previous frame key states

    void CopyKeyMap(); // Helper function to copy current key states to previous
};

#endif // INPUTMANAGER_H
