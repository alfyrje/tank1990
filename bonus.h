#ifndef bonus_h
#define bonus_h

#include "object.h"

class Bonus : public Object {
public:
    Bonus();
    Bonus(double x, double y, OBJECT_TYPE type);
    void draw(Renderer* renderer);
    void update(int dt);
    int bonusShowTime;
    bool show;
    int blinkFrame;
};

#endif
