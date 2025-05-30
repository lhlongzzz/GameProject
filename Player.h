#pragma once
#include <SDL2/SDL.h>
#include <bits/stdc++.h>

class Bullet;

class Player {
public:
    Player(SDL_Renderer* renderer);
    ~Player();

    void handleInput(const Uint8* keyState);
    void update();
    void render();

    SDL_Rect getRect();

    std::vector<Bullet*>& getBullets();
    int getHp() const;
    void loseHp();
    void reset();

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect rect;
    int speed;
    int hp;

    std::vector<Bullet*> bullets;
    Uint32 lastShotTime;
    Uint32 shootCooldown;
};