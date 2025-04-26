#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>

class Enemy {
    public:
        Enemy(SDL_Renderer* renderer, int x, int y, SDL_Texture* texture, bool isSpecial = false);
        ~Enemy();
    
        void update();                // Di chuyển xuống
        void render();                // Vẽ gà lên màn hình
    
        SDL_Rect getRect() const;     // Lấy vùng va chạm
        bool isOffScreen() const;     // Kiểm tra nếu đã ra khỏi màn hình
        bool isSpecialEnemy() const;

        void takeDamage(int damage);
        int getHP() const;
    
    private:
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        SDL_Rect rect;
        int hp;
        int speed;
        bool special;
    };
#endif
