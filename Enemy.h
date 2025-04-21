#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>

class Enemy {
public:
    Enemy(SDL_Renderer* renderer, int x, int y, SDL_Texture* tex, bool isSpecial = false);

    void update();         // Di chuyển xuống
    void render();         // Vẽ gà lên màn hình
    SDL_Rect getRect();    // Lấy vùng va chạm
    bool isOutOfScreen();  // Kiểm tra nếu đã ra khỏi màn hình
    bool isSpecialChicken();

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect rect;
    int speed;
    bool special;
};

#endif
