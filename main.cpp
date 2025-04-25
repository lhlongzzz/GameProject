#include <bits/stdc++.h>

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>



using namespace std;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Chicken Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* chickenTexture = IMG_LoadTexture(renderer, "assets/enemy.png");
    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
    SDL_Texture* bulletTexture = IMG_LoadTexture(renderer, "assets/bullet.png");

    vector<Enemy*> enemies;
    vector<Bullet*> bullets;

    SDL_Rect playerRect = { 400, 500, 64, 64 };
    SDL_Event event;
    bool running = true;
    Uint32 lastEnemySpawnTime = 0;
    Uint32 lastShootTime = 0;

    srand(time(0)); // Seed for random number generation

    Player player(renderer);

    while (running) {
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        if (keyState[SDL_SCANCODE_LEFT]) playerRect.x -= 5;
        if (keyState[SDL_SCANCODE_RIGHT]) playerRect.x += 5;
        if (keyState[SDL_SCANCODE_UP]) playerRect.y -= 5;
        if (keyState[SDL_SCANCODE_DOWN]) playerRect.y += 5;

        if (keyState[SDL_SCANCODE_SPACE] && SDL_GetTicks() - lastShootTime > 100) {
            Bullet* bullet = new Bullet(renderer, playerRect.x + playerRect.w / 2, playerRect.y);
            bullets.push_back(bullet);
            lastShootTime = SDL_GetTicks();
        }

        if (SDL_GetTicks() - lastEnemySpawnTime > 1000) {
            int x = rand() % (800-64); // Random x position
            bool isSpecial = rand() % 10; // Randomly decide if it's a special enemy
            Enemy* newEnemy = new Enemy(renderer, x, -64, chickenTexture, isSpecial);
            enemies.push_back(newEnemy);
            lastEnemySpawnTime = SDL_GetTicks();
        }

        
        for (auto& e : enemies) e->update();
        for (auto& b : bullets) b->update();
        
        for (int i = 0; i < enemies.size(); ++i) {
            if (enemies[i]->isOffScreen()) {
                delete enemies[i];
                enemies.erase(enemies.begin() + i);
                --i;
            }
        }
        for (int i = 0; i < bullets.size(); ++i) {
            if (bullets[i]->isOffScreen()) {
                delete bullets[i];
                bullets.erase(bullets.begin() + i);
                --i;
            }
        }
        
        player.handleInput(keyState);
        player.update();
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        for (auto& e : enemies) {
            e->render();
        }
        
        player.render();
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}