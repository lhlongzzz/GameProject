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
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window* window = SDL_CreateWindow("Chicken Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "assets/background.png");
    SDL_Texture* chickenTexture = IMG_LoadTexture(renderer, "assets/enemy.png");
    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
    SDL_Texture* bulletTexture = IMG_LoadTexture(renderer, "assets/bullet.png");
    SDL_Texture* heartTexture = IMG_LoadTexture(renderer, "assets/heart.png");

    TTF_Font* font = TTF_OpenFont("assets/arial.ttf", 28);

    Mix_Chunk* chickenDieSound = Mix_LoadWAV("assets/chicken_die.oga");
    Mix_Chunk* playerHitSound = Mix_LoadWAV("assets/player_hit.oga");
    Mix_Chunk* gameOverSound = Mix_LoadWAV("assets/game_over.oga");
    Mix_Music* mainTheme = Mix_LoadMUS("assets/maintheme.oga");

    Mix_VolumeChunk(playerHitSound, 32);

    vector<Enemy*> enemies;

    SDL_Rect playerRect = { 400, 500, 64, 64 };
    SDL_Event event;
    bool running = true;
    bool inMenu = true;
    bool gameOver = false;
    int score = 0;
    Uint32 lastEnemySpawnTime = 0;

    srand(time(0));

    Player player(renderer);

    Uint32 lastToggleTime = 0;
    bool showText = true;

    if (mainTheme) {
        Mix_PlayMusic(mainTheme, -1);
    }

    while (inMenu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                inMenu = false;
            }
        }

        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        if (keyState[SDL_SCANCODE_SPACE]) {
            inMenu = false;
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastToggleTime >= 500) {
            showText = !showText;
            lastToggleTime = currentTime;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (showText) {
            SDL_Color white = {255, 255, 255, 255};
            TTF_Font* menuFont = TTF_OpenFont("assets/arial.ttf", 36);
            SDL_Surface* textSurface = TTF_RenderText_Solid(menuFont, "PRESS SPACE TO START", white);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            int textW = 0, textH = 0;
            SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
            SDL_Rect textRect = {400 - textW / 2, 300 - textH / 2, textW, textH};

            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
            TTF_CloseFont(menuFont);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    while (running) {
        const Uint8* keyState = SDL_GetKeyboardState(NULL);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        if (gameOver) {
            // Hiển thị màn hình Game Over
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
        
            SDL_Color white = {255, 255, 255, 255};
            TTF_Font* endFont = TTF_OpenFont("assets/arial.ttf", 36);
        
            // Hiển thị "GAME OVER"
            SDL_Surface* endTextSurface = TTF_RenderText_Solid(endFont, "GAME OVER", white);
            SDL_Texture* endTextTexture = SDL_CreateTextureFromSurface(renderer, endTextSurface);
            int textW = 0, textH = 0;
            SDL_QueryTexture(endTextTexture, NULL, NULL, &textW, &textH);
            SDL_Rect textRect = {400 - textW / 2, 250 - textH / 2, textW, textH};
            SDL_RenderCopy(renderer, endTextTexture, NULL, &textRect);
            SDL_FreeSurface(endTextSurface);
            SDL_DestroyTexture(endTextTexture);
        
            // Hiển thị "Press R to Restart" với hiệu ứng nhấp nháy
            Uint32 currentTime = SDL_GetTicks();
            static Uint32 lastRestartToggleTime = 0;
            static bool showRestartText = true;
        
            if (currentTime - lastRestartToggleTime >= 500) {
                showRestartText = !showRestartText;
                lastRestartToggleTime = currentTime;
            }
        
            if (showRestartText) {
                SDL_Surface* restartTextSurface = TTF_RenderText_Solid(endFont, "Press R to Restart", white);
                SDL_Texture* restartTextTexture = SDL_CreateTextureFromSurface(renderer, restartTextSurface);
                SDL_QueryTexture(restartTextTexture, NULL, NULL, &textW, &textH);
                SDL_Rect restartTextRect = {400 - textW / 2, 350 - textH / 2, textW, textH};
                SDL_RenderCopy(renderer, restartTextTexture, NULL, &restartTextRect);
                SDL_FreeSurface(restartTextSurface);
                SDL_DestroyTexture(restartTextTexture);
            }
        
            TTF_CloseFont(endFont);
        
            SDL_RenderPresent(renderer);
        
            if (keyState[SDL_SCANCODE_R]) {
                gameOver = false;
                score = 0;
                player.reset();
                for (auto& e : enemies) delete e;
                enemies.clear();
                lastEnemySpawnTime = SDL_GetTicks();
            }
        
            SDL_Delay(16);
            continue;
        }

        player.handleInput(keyState);

        if (SDL_GetTicks() - lastEnemySpawnTime > 1000) {
            int x = rand() % (800 - 64);
            bool isSpecial = rand() % 5;
            Enemy* newEnemy = new Enemy(renderer, x, -64, chickenTexture, isSpecial);
            enemies.push_back(newEnemy);
            lastEnemySpawnTime = SDL_GetTicks();
        }

        for (auto& e : enemies) e->update();
        player.update();
        
        for (int i = 0; i < enemies.size(); ++i) {
            SDL_Rect playerRect = player.getRect();
            SDL_Rect enemyRect = enemies[i]->getRect();
            if (SDL_HasIntersection(&playerRect, &enemyRect)) {
                Mix_PlayChannel(-1, playerHitSound, 0);
                player.loseHp();
                delete enemies[i];
                enemies.erase(enemies.begin() + i);
                --i;
    
                if (player.getHp() <= 0) {
                    gameOver = true;
                    Mix_PlayChannel(-1, gameOverSound, 0);
                }
            }
        }

        for (int i = 0; i < enemies.size(); ++i) {
            if (enemies[i]->isOffScreen()) {
                delete enemies[i];
                enemies.erase(enemies.begin() + i);
                --i;
            }
        }
        
        auto& bullets = player.getBullets();
        
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
                        Mix_PlayChannel(-1, chickenDieSound, 0);
                        score += 100;
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

        for (int i = 0; i < player.getHp(); ++i) {
            SDL_Rect heartRect = {10 + i * 40, 50, 32, 32};
            SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
        }

        SDL_Color white = {255, 255, 255, 255};
        string scoreText = "Score: " + to_string(score);
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

        SDL_Rect scoreRect = {10, 10, scoreSurface->w, scoreSurface->h};
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    for (auto& e : enemies) delete e;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}