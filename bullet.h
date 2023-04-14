#ifndef bullet_h
#define bullet_h

#include "object.h"
#include<SDL.h>

class Bullet : public Object {
public:
    Bullet();
    Bullet(double x, double y);
    void update(int dt);
    bool increased_damage;
    void draw(Renderer* renderer);
    int direction;
    double speed;
    bool collide;
    void destroy();

};

#endif
