#include "eagle.h"
#include<iostream>

Eagle::Eagle() {
    pos_x = 0.0;
    pos_y = 0.0;
    to_erase = false;
    type = 0;
    destroyTime = 0;
    destroyFrame = 0;
    flag = 0;
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
    destroyTime = 0;
    destroyFrame = 0;
    flag = 0;
}

void Eagle::update(int dt) {
    if(type) {
        destroyTime += dt;
        if(destroyTime > 100) {
            destroyTime = 0;
            destroyFrame++;
            src_rect = moveRect(src_rect, 0, destroyFrame);
            if((!flag && destroyFrame >= 4) || (flag && destroyFrame >= 1)) {
                src_rect = {944, 32, 32, 32};
                dest_rect.x = pos_x;
                dest_rect.y = pos_y;
                dest_rect.h = 40;
                dest_rect.w = 40;
                destroyFrame = 0;
                flag = 1;
            }
        }
    }
    else Object::update(dt);
}

void Eagle::destroy() {
    if(type) return;
    type = true;

    src_rect = {1040, 0, 64, 64};

    dest_rect.x = pos_x - 20;
    dest_rect.y = pos_y - 20;
    dest_rect.h = 80;
    dest_rect.w = 80;
}
