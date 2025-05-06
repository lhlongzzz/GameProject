#include "Player.h"
#include "Bullet.h"
#include <SDL2/SDL_image.h>

Player::Player(SDL_Renderer* renderer) : renderer(renderer), speed(5), lastShotTime(0), shootCooldown(250) {
    SDL_Surface* surface = IMG_Load("assets/player.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    hp = 3;
    rect.w = 64;
    rect.h = 64;
    rect.x = 400 - rect.w / 2;
    rect.y = 500;
}

Player::~Player() {
    SDL_DestroyTexture(texture);
    for (auto bullet : bullets) delete bullet;
}

void Player::handleInput(const Uint8* keyState) {
    if (keyState[SDL_SCANCODE_W]) rect.y -= speed;
    if (keyState[SDL_SCANCODE_S]) rect.y += speed;
    if (keyState[SDL_SCANCODE_A]) rect.x -= speed;
    if (keyState[SDL_SCANCODE_D]) rect.x += speed;

    if (rect.x < 0) rect.x = 0;
    if (rect.x + rect.w > 800) rect.x = 800 - rect.w;
    if (rect.y < 0) rect.y = 0;
    if (rect.y + rect.h > 600) rect.y = 600 - rect.h;

    Uint32 currentTime = SDL_GetTicks();
    if (keyState[SDL_SCANCODE_SPACE] && currentTime - lastShotTime >= shootCooldown) {
        bullets.push_back(new Bullet(renderer, rect.x + rect.w / 2 - 4, rect.y));
        lastShotTime = currentTime;
    }
}

void Player::update() {
    for (auto bullet : bullets) bullet->update();

    // Xoá đạn ra khỏi màn hình
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](Bullet* b) {
            if (b->isOffScreen()) {
                delete b;
                return true;
            }
            return false;
        }), bullets.end());
}

void Player::render() {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    for (auto bullet : bullets) bullet->render();
}

SDL_Rect Player::getRect() {
    return rect;
}

std::vector<Bullet*>& Player::getBullets() {
    return bullets;
}

int Player::getHp() const {
    return hp;
}

void Player::loseHp() {
    if (hp > 0) hp--;
}

void Player::reset() {
    hp = 3;
    rect = {400, 500, 64, 64};
    for (auto& bullet : bullets) delete bullet;
    bullets.clear();
    lastShotTime = 0;
}