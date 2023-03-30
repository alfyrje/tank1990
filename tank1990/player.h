#ifndef player_h
#define player_h

#include "tank.h"
#include<SDL.h>

class Player : public Tank {
public:
    Player();
    ~Player();
    int stars;
    int fireTime;
    int maxBullet;
    void update(int dt);
    Bullet* fire();
};

#endif
