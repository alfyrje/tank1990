#ifndef player_h
#define player_h

#include "tank.h"
#include<SDL.h>

class Player : public Tank {
public:
    struct playerKeys {
        SDL_Scancode up, right, down, left;
    };

    Player();

    int stars;
    int fireTime;
    int maxBullet;
    void update(int dt);
    Bullet* fire();
};

#endif
