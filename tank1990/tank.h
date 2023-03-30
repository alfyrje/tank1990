#ifndef tank_h
#define tank_h

#include "object.h"
#include "bullet.h"
#include<vector>

class Tank : public Object {
public:
    Tank();
    virtual ~Tank();
    void draw();
    void update(int dt);
    virtual Bullet* fire();
    SDL_Rect nextCollisionRect(int dt);
    void setDirection(int d);
    void collide();
    int src_x;
    int direction;
    void collide(SDL_Rect &intersect_rect);
    double speed;
    bool stop;
    int lives;
    std::vector<Bullet*> bullets;
};


#endif
