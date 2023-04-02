#include "tank.h"
#include "gameconfig.h"
#include "renderer.h"

#include<algorithm>

Tank::Tank() {
    direction = 0;
    speed = 0.0;
    to_erase = false;
}

Tank::~Tank() {
    for(auto bullet : bullets) delete bullet;
    bullets.clear();
}

void Tank::draw() {
    if(to_erase) return;
    Object::draw();
    for(auto bullet : bullets) if(bullet != nullptr) bullet->draw();
}

void Tank::update(int dt) {
    if(to_erase) return;
    if(!stop) {
        switch(direction) {
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

        collision_rect.x = pos_x + 2.5;
        collision_rect.y = pos_y + 2.5;
        collision_rect.h = dest_rect.h - 5;
        collision_rect.w = dest_rect.w - 5;
    }

    for(auto bullet : bullets) bullet->update(dt);
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet*b){if(b->to_erase) {delete b; return true;} return false;}), bullets.end());
}

Bullet* Tank::fire() {
    if((int)bullets.size() < GameConfig::max_bullet) {
        Bullet* bullet = new Bullet(pos_x, pos_y);
        bullets.push_back(bullet);
        switch(direction)
        {
        case 0:
            bullet->pos_x += (dest_rect.w - bullet->dest_rect.w) / 2;
            bullet->pos_y -= bullet->dest_rect.h - 4;
            break;
        case 1:
            bullet->pos_x += dest_rect.w - 4;
            bullet->pos_y += (dest_rect.h - bullet->dest_rect.h) / 2;
            break;
        case 2:
            bullet->pos_x += (dest_rect.w - bullet->dest_rect.w) / 2;
            bullet->pos_y += dest_rect.h - 4;
            break;
        case 3:
            bullet->pos_x -= bullet->dest_rect.w - 4;
            bullet->pos_y += (dest_rect.h - bullet->dest_rect.h) / 2;
            break;
        }
        bullet->direction = direction;
        bullet->src_rect.x = 944 + direction * 8;
        bullet->speed = GameConfig::bullet_default_speed;

        bullet->update(0);

        return bullet;
    }
    return nullptr;
}


void Tank::setDirection(int d) {
    direction = d;
    src_rect.x = src_x + d * 32;
}

SDL_Rect Tank::nextCollisionRect(int dt) {
    if(speed == 0) return collision_rect;

    SDL_Rect r;
    int a = 1;
    switch (direction) {
    case 0:
        r.x = collision_rect.x;
        r.y = collision_rect.y - GameConfig::tank_default_speed * dt - a;
        break;
    case 1:
        r.x = collision_rect.x + GameConfig::tank_default_speed * dt + a;
        r.y = collision_rect.y;
        break;
    case 2:
        r.x = collision_rect.x;
        r.y = collision_rect.y + GameConfig::tank_default_speed * dt + a;
        break;
    case 3:
        r.x = collision_rect.x - GameConfig::tank_default_speed * dt - a;
        r.y = collision_rect.y;
        break;
    }
    r.h = collision_rect.h;
    r.w = collision_rect.w;

    return r;
}

void Tank::collide(SDL_Rect &intersect_rect) {
    if(intersect_rect.w > intersect_rect.h)
    {
        if((direction == 0 && intersect_rect.y <= collision_rect.y) ||
                (direction == 2 && (intersect_rect.y + intersect_rect.h) >= (collision_rect.y + collision_rect.h)))
        {
            stop = true;
        }
    }
    else
    {
        if((direction == 3 && intersect_rect.x <= collision_rect.x) ||
                (direction == 1 && (intersect_rect.x + intersect_rect.w) >= (collision_rect.x + collision_rect.w)))
        {
            stop = true;
        }
    }
}
