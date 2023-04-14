#include "object.h"
#include "renderer.h"

#include<algorithm>
#include<SDL.h>


Object::Object() {
    pos_x = 0.0;
    pos_y = 0.0;
    to_erase = false;
}

Object::Object(double x, double y, OBJECT_TYPE type) {
    pos_x = x;
    pos_y = y;
    to_erase = false;
    TYPE = type;
    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
    dest_rect.w = 40;
    dest_rect.h = 40;
    if(type == STONE_WALL) {
        src_rect = {928, 144, 16, 16};
        dest_rect.h = 20;
        dest_rect.w = 20;
    }
    else if(type == BUSH) {
        src_rect = {928, 192, 16, 16};
        dest_rect.w = 20;
        dest_rect.h = 20;
    }
    else if(type == SHIELD) src_rect = {976, 0, 32, 32};
    else if(type == BONUS_GRENADE) src_rect = {896, 0, 32, 32};
    else if(type == BONUS_HELMET) src_rect = {896, 32, 32, 32};
    else if(type == BONUS_CLOCK) src_rect = {896, 64, 32, 32};
    else if(type == BONUS_TANK) src_rect = {896, 128, 32, 32};
    else if(type == BONUS_STAR) src_rect = {896, 160, 32, 32};
    else if(type == BONUS_GUN) src_rect = {896, 192, 32, 32};

    collision_rect = dest_rect;
}

Object::~Object() {
}

void Object::draw(Renderer* renderer) {
    if(to_erase) return;
    renderer->drawObject(&src_rect, &dest_rect);
}

void Object::update(int dt) {
    if(to_erase) return;

    dest_rect.x = pos_x;
    dest_rect.y = pos_y;

    collision_rect.x = dest_rect.x;
    collision_rect.y = dest_rect.y;
    collision_rect.w = dest_rect.w;
    collision_rect.h = dest_rect.h;
}

SDL_Rect Object::moveRect(const SDL_Rect &rect, int x, int y) {
    SDL_Rect r;
    r.x = rect.x + x*rect.w;
    r.y = rect.y + y*rect.h;
    r.w = rect.w;
    r.h = rect.h;

    return r;
}
