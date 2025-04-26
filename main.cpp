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

    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "assets/background.png");
    SDL_Texture* chickenTexture = IMG_LoadTexture(renderer, "assets/enemy.png");
    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
    SDL_Texture* bulletTexture = IMG_LoadTexture(renderer, "assets/bullet.png");

    vector<Enemy*> enemies;

    SDL_Rect playerRect = { 400, 500, 64, 64 };
    SDL_Event event;
    bool running = true;
    Uint32 lastEnemySpawnTime = 0;

    srand(time(0));

    Player player(renderer);

    while (running) {
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        player.handleInput(keyState);

        if (SDL_GetTicks() - lastEnemySpawnTime > 1000) {
            int x = rand() % (800 - 64);
            bool isSpecial = rand() % 10;
            Enemy* newEnemy = new Enemy(renderer, x, -64, chickenTexture, isSpecial);
            enemies.push_back(newEnemy);
            lastEnemySpawnTime = SDL_GetTicks();
        }

        for (auto& e : enemies) e->update();
        player.update();

        // Kiểm tra enemy out of screen
        for (int i = 0; i < enemies.size(); ++i) {
            if (enemies[i]->isOffScreen()) {
                delete enemies[i];
                enemies.erase(enemies.begin() + i);
                --i;
            }
        }

        // Lấy bullets từ player
        auto& bullets = player.getBullets();

        // Va chạm đạn và gà
        for (int i = 0; i < bullets.size(); ++i) {
            SDL_Rect bulletRect = bullets[i]->getRect();
            for (int j = 0; j < enemies.size(); ++j) {
                SDL_Rect enemyRect = enemies[j]->getRect();
                if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                    enemies[j]->takeDamage(1);
                    delete bullets[i];
                    bullets.erase(bullets.begin() + i);
                    --i;
                    if (enemies[j]->getHP() <= 0) {
                        delete enemies[j];
                        enemies.erase(enemies.begin() + j);
                    }
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        for (auto& e : enemies) {
            e->render();
        }

        player.render();

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    for (auto& e : enemies) delete e;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
