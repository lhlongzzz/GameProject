#include "Enemy.h"

Enemy::Enemy(SDL_Renderer* renderer, int x, int y, SDL_Texture* tex, bool isSpecial)
    : renderer(renderer), texture(tex), special(isSpecial)
{
    rect = { x, y, 64, 64 }; // kích thước gà
    speed = special ? 2 : 3; // gà đặc biệt bay chậm hơn chút
}

void Enemy::update() {
    rect.y += speed;
}

void Enemy::render() {
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

SDL_Rect Enemy::getRect() {
    return rect;
}

bool Enemy::isOutOfScreen() {
    return rect.y > 600; // giả sử màn hình cao 600px
}

bool Enemy::isSpecialChicken() {
    return special;
}