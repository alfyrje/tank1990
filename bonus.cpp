#include "bonus.h"
#include "object.h"
#include "gameconfig.h"

Bonus::Bonus() : Object(0, 0, BONUS_STAR) {
    bonusShowTime = 0;
    show = 1;
    blinkFrame = 0;
}

Bonus::Bonus(double x, double y, OBJECT_TYPE type) : Object(x, y, type) {
    bonusShowTime = 0;
    show = 1;
    blinkFrame = 0;
}

void Bonus::draw(Renderer* renderer) {
    if(show) Object::draw(renderer);
}

void Bonus::update(int dt) {
    Object::update(dt);
    bonusShowTime += dt;
    if(bonusShowTime > GameConfig::bonus_show_time)
        to_erase = true;

    if(bonusShowTime > blinkFrame * 350) {
        blinkFrame++;
        show = blinkFrame % 2;
    }
}
