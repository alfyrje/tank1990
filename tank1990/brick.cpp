#include "brick.h"

#include<iostream>

Brick::Brick() {
    pos_x = 0.0;
    pos_y = 0.0;
    to_erase = false;
    collisionCount = 0;
    state = 0;
}

Brick::Brick(double x, double y) {
    collisionCount = 0;
    state = 0;
    pos_x = x;
    pos_y = y;
    to_erase = false;
    src_rect = {928, 0, 16, 16};
    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
    dest_rect.w = 20;
    dest_rect.h = 20;
    collision_rect = dest_rect;
    TYPE = BRICK_WALL;
}

void Brick::update(int dt) {

}

void Brick::bulletHit(int direction) {
    collisionCount++;
    if(collisionCount == 1) state = direction + 1;
    else if(collisionCount == 2) {
        if((state - 1 + direction) % 2 == 0) {
            state = 9;
            to_erase = true;
        }
        else if((state == 1 && direction + 1 == 2) || (state == 2 && direction + 1 == 1)) {
            state = 5;
        }
        else if((state == 2 && direction + 1 == 3) || (state == 3 && direction + 1 == 2)) {
            state = 6;
        }
        else if((state == 1 && direction + 1 == 4) || (state == 4 && direction + 1 == 1)) {
            state = 7;
        }
        else if((state == 3 && direction + 1 == 4) || (state == 4 && direction + 1 == 3)) {
            state = 8;
        }
    }
    else {
        state = 9;
        to_erase = true;
    }

    switch(state) {
    case 1:
        collision_rect.h /= 2;
        break;
    case 2:
        collision_rect.x = pos_x + 10;
        collision_rect.w /= 2;
        break;
    case 3:
        collision_rect.y = pos_y + 10;
        collision_rect.h /= 2;
        break;
    case 4:
        collision_rect.w /= 2;
        break;
    case 5:
        collision_rect.x = pos_x + 10;
        collision_rect.h /= 2;
        collision_rect.w /= 2;
        break;
    case 6:
        collision_rect.x = pos_x + 10;
        collision_rect.y = pos_y + 10;
        collision_rect.h /= 2;
        collision_rect.w /= 2;
        break;
    case 7:
        collision_rect.h /= 2;
        collision_rect.w /= 2;
        break;
    case 8:
        collision_rect.y = pos_y + 10;
        collision_rect.h /= 2;
        collision_rect.w /= 2;
        break;
    case 9:
        collision_rect.x = 0;
        collision_rect.y = 0;
        collision_rect.h = 0;
        collision_rect.w = 0;
        break;
    }

    src_rect.y = 16 * state;
}
