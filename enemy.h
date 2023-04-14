#ifndef enemy_h
#define enemy_h

#include "tank.h"

class Enemy : public Tank {
public:
    Enemy();
    Enemy(double x, double y, OBJECT_TYPE type);
    Enemy(double x, double y, int type);
    void draw(Renderer* renderer);
    void update(int dt);
    bool bonusFlag;
    void destroy();
    int directionTime;
    int keepDirectionTime;
    int reloadTime;
    int speedTime;
    int tryTime;
    SDL_Point targetPos;
};

#endif
