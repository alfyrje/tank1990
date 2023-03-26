#ifndef bullet_h
#define bullet_h

#include<SDL.h>

class Bullet {
public:
    Bullet();
    Bullet(double x, double y);
    ~Bullet();
    void update(int dt);
    bool increased_damage;
    void draw();
    int direction;
    double speed;
    double pos_x;
    double pos_y;
    bool to_erased;
    SDL_Rect src_rect;
    SDL_Rect dest_rect;
};

#endif
