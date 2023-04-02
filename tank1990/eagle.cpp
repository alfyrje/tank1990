#include "eagle.h"

Eagle::Eagle() {
    pos_x = 0.0;
    pos_y = 0.0;
    to_erase = false;
    type = 0;
}

Eagle::Eagle(double x, double y) {
    pos_x = x;
    pos_y = y;
    to_erase = false;
    src_rect = {944, 0, 32, 32};
    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
    dest_rect.w = 40;
    dest_rect.h = 40;
    collision_rect = dest_rect;
    type = 0;
}

void Eagle::update(int dt) {
    if(type) {

    }
    else Object::update(dt);
}

void Eagle::destroy() {

}
