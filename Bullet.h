#pragma once
#include <SDL2/SDL.h>

class Bullet {
public:
    Bullet(SDL_Renderer* renderer, int x, int y);
    ~Bullet();

    void update();
    void render();
    bool isOffScreen();
    SDL_Rect getRect();

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect rect;
    int speed;
};