#include "Enemy.h"

Enemy::Enemy(SDL_Renderer* renderer, int x, int y, SDL_Texture* texture, bool isSpecial)
    : renderer(renderer), texture(texture), special(isSpecial)
{
    rect = { x, y, 64, 64 }; // kích thước gà
    speed = 2;
    hp = special ? 2 : 1;
}

Enemy::~Enemy() {
    // Không cần SDL_DestroyTexture vì texture được quản lý bên ngoài
}

void Enemy::update() {
    rect.y += speed;
}

void Enemy::render() {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

SDL_Rect Enemy::getRect() const {
    return rect;
}

bool Enemy::isOffScreen() const {
    return rect.y > 600; // chiều cao màn hình
}

bool Enemy::isSpecialEnemy() const {
    return special;
}

void Enemy::takeDamage(int damage) {
    hp -= damage;
}

int Enemy::getHP() const {
    return hp;
}