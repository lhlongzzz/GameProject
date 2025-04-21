#include "Bullet.h"
#include <SDL2/SDL_image.h>

Bullet::Bullet(SDL_Renderer* renderer, int x, int y) : renderer(renderer), speed(10) {
    SDL_Surface* surface = IMG_Load("assets/bullet.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    rect.w = 8;
    rect.h = 16;
    rect.x = x;
    rect.y = y;
}

Bullet::~Bullet() {
    SDL_DestroyTexture(texture);
}

void Bullet::update() {
    rect.y -= speed;
}

void Bullet::render() {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

bool Bullet::isOffScreen() {
    return rect.y + rect.h < 0;
}

SDL_Rect Bullet::getRect() {
    return rect;
}