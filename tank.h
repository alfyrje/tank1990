#ifndef tank_h
#define tank_h

#include "object.h"
#include "bullet.h"
#include<vector>

class Tank : public Object {
public:
    Tank();
    virtual ~Tank();
    virtual void draw(Renderer* renderer);
    virtual void update(int dt);
    virtual Bullet* fire();
    SDL_Rect nextCollisionRect(int dt);
    void setDirection(int d);
    void collide();
    int direction;
    void collide(SDL_Rect &intersect_rect);
    double speed;
    bool stop;
    int lives;
    int fireTime;
    int maxBullet;
    double defaultSpeed;
    std::vector<Bullet*> bullets;
    virtual void respawn();
    virtual void destroy();
    bool spawnFlag;
    int spawnTime;
    int spawnFrame;
    Object* shield;
    void grantShield();
    int shieldTime = 0;
    bool shieldFlag;
    int shieldFrame;
    int frozenTime;
    bool frozenFlag;
};


#endif
