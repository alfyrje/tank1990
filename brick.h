#ifndef brick_h
#define brick_h

#include "object.h"

class Brick : public Object {
public:
    Brick();
    Brick(double x, double y);

    void update(int dt);
    void bulletHit(int direction);
    int state;
    int collisionCount;
};

#endif
