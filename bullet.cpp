#include "bullet.h"
#include "renderer.h"

#include<iostream>

Bullet::Bullet() {
    to_erase = false;
    increased_damage = false;
    speed = 0.0;
    direction = 0;
    collide = 0;
    destroyTime = 0;
    destroyFrame = 0;
}

Bullet::Bullet(double x, double y) {
    to_erase = false;
    increased_damage = false;
    speed = 0.0;
    direction = 0;
    pos_x = x; pos_y = y;
    dest_rect.x = x;
    dest_rect.y = y;
    dest_rect.w = 10;
    dest_rect.h = 10;
    collision_rect.x = x;
    collision_rect.y = y;
    collision_rect.w = 10;
    collision_rect.h = 10;
    src_rect = {944, 128, 8, 8};
    collide = 0;
    destroyTime = 0;
    destroyFrame = 0;
}

void Bullet::update(int dt) {
    if(!collide) {
        switch (direction)
        {
        case 0:
            pos_y -= speed * dt;
            break;
        case 1:
            pos_x += speed * dt;
            break;
        case 2:
            pos_y += speed * dt;
            break;
        case 3:
            pos_x -= speed * dt;
            break;
        }
        dest_rect.x = pos_x;
        dest_rect.y = pos_y;

        collision_rect.x = pos_x;
        collision_rect.y = pos_y;
    }
    else {
        destroyTime += dt;
        if(destroyTime > 40) {
            destroyTime = 0;
            destroyFrame++;
            if(destroyFrame >= 5) to_erase = true;
            src_rect = moveRect(src_rect, 0, destroyFrame);
        }
    }
}

void Bullet::draw(Renderer* renderer) {
    renderer->drawObject(&src_rect, &dest_rect);
}

void Bullet::destroy() {
    if(collide) return;

    collide = true;
    speed = 0;

    switch(direction) {
    case 0:
        dest_rect.x = pos_x - (40 - dest_rect.w) / 2;
        dest_rect.y = pos_y - 40 / 2;
        break;
    case 1:
        dest_rect.x = pos_x + dest_rect.w - 40 / 2;
        dest_rect.y = pos_y + (dest_rect.h - 40) / 2;
        break;
    case 2:
        dest_rect.x = pos_x + (dest_rect.w - 40) / 2;
        dest_rect.y = pos_y + dest_rect.h - 40 / 2;
        break;
    case 3:
        dest_rect.x = pos_x - 40 / 2;
        dest_rect.y = pos_y - (40 - dest_rect.h) / 2;
        break;
    }

    dest_rect.h = 40;
    dest_rect.w = 40;

    src_rect.x = 1108;
    src_rect.y = 0;
    src_rect.h = 32;
    src_rect.w = 32;

    update(0);
}
