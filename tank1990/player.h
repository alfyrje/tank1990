#ifndef player_h
#define player_h

#include "bullet.h"
#include<vector>
#include<SDL.h>

class Player {
public:
    Player();
    ~Player();
    int lives;
    int stars;
    int direction;
    int fireTime;
    int maxBullet;
    double speed;
    double pos_x;
    double pos_y;
    SDL_Rect dest_rect;
    SDL_Rect src_rect;
    void update(int dt);
    void draw();
    void setDirection(int d);
    Bullet* fire();
    std::vector<Bullet*> bullets;
};

#endif
