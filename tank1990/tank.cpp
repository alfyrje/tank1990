#include "tank.h"
#include "gameconfig.h"
#include "renderer.h"

#include<iostream>
#include<algorithm>

Tank::Tank() {
    direction = 0;
    speed = 0.0;
    to_erase = false;
    destroyFlag = false;
    spawnFlag = 0;
}

Tank::~Tank() {
    for(auto bullet : bullets) delete bullet;
    bullets.clear();
}

void Tank::draw() {
    if(to_erase) return;
    Object::draw();
    if(shieldFlag && shield != nullptr) shield->draw();
    for(auto bullet : bullets) if(bullet != nullptr) bullet->draw();
}

void Tank::update(int dt) {
    if(to_erase) return;

    if(lives <= 0) destroy();
    if(!destroyFlag && !spawnFlag) {
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
    }

    else if(spawnFlag) {
        spawnTime += dt;
        if(spawnTime > 100) {
            spawnTime = 0;
            spawnFrame++;
            src_rect = moveRect(src_rect, 0, 1);
            if(spawnFrame >= 10) {
                spawnFlag = false;
                src_rect = pre_src_rect;
                src_x = pre_src_rect.x;
                spawnFrame = 0;
            }
        }
    }
    else if(destroyFlag) {
        destroyTime += dt;
        if(destroyTime > 70) {
            destroyTime = 0;
            destroyFrame++;
            src_rect = moveRect(src_rect, 0, 1);
            if(destroyFrame >= 7) {
                destroyFlag = false;
                destroyFrame = 0;
                src_rect = pre_src_rect;
                src_x = pre_src_rect.x;
                dest_rect.x = pos_x + 20;
                dest_rect.y = pos_y + 20;
                dest_rect.w = 40;
                dest_rect.h = 40;
                if(lives > 0) respawn();
                else if(bullets.empty()) to_erase = true;
            }
        }
    }

    if(shieldFlag && shield != nullptr) {
        shieldTime += dt;
        shield->pos_x = pos_x;
        shield->pos_y = pos_y;
        shield->update(dt);
        if(shieldTime > shieldFrame * 100) {
            shieldFrame++;
            shield->src_rect.y = (shieldFrame % 2) * 32;
        }
        if(shieldTime > GameConfig::tank_shield_time) {
            shieldFlag = 0;
            delete shield;
            shield = nullptr;
            shieldTime = 0;
            shieldFrame = 0;
        }
    }

    for(auto bullet : bullets) bullet->update(dt);
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet*b){if(b->to_erase) {delete b; return true;} return false;}), bullets.end());
}

Bullet* Tank::fire() {
    if(destroyFlag || spawnFlag) return nullptr;

    if((int)bullets.size() < maxBullet) {
        Bullet* bullet = new Bullet(pos_x, pos_y);
        bullets.push_back(bullet);
        switch(direction)
        {
        case 0:
            bullet->pos_x += (dest_rect.w - bullet->dest_rect.w) / 2;
            bullet->pos_y -= bullet->dest_rect.h - 5;
            break;
        case 1:
            bullet->pos_x += dest_rect.w - 5;
            bullet->pos_y += (dest_rect.h - bullet->dest_rect.h) / 2;
            break;
        case 2:
            bullet->pos_x += (dest_rect.w - bullet->dest_rect.w) / 2;
            bullet->pos_y += dest_rect.h - 5;
            break;
        case 3:
            bullet->pos_x -= bullet->dest_rect.w - 5;
            bullet->pos_y += (dest_rect.h - bullet->dest_rect.h) / 2;
            break;
        }
        bullet->direction = direction;
        bullet->src_rect.x = 944 + direction * 8;

        if(TYPE == TANK_C)
            bullet->speed = GameConfig::bullet_default_speed * 1.3;
        else
            bullet->speed = GameConfig::bullet_default_speed;

        bullet->update(0);

        return bullet;
    }
    return nullptr;
}


void Tank::setDirection(int d) {
    direction = d;
    src_rect.x = src_x + d * src_rect.w;
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


void Tank::respawn() {
    pre_src_rect = src_rect;
    src_rect = {1008, 0, 32, 32};
    src_x = 1008;
    speed = 0.0;

    stop = false;

    spawnFlag = 1;
    spawnFrame = 0;
    spawnTime = 0;

    collision_rect.x = 0;
    collision_rect.y = 0;
    collision_rect.h = 0;
    collision_rect.w = 0;

    update(0);
}

void Tank::destroy() {
    if(destroyFlag || spawnFlag || shieldFlag) return;
    stop = true;
    destroyFlag = true;
    destroyFrame = 0;
    destroyTime = 0;
    setDirection(0);
    speed = 0;

    pre_src_rect = src_rect;
    src_rect = {1040, 0, 64, 64};
    src_x = 1040;

    dest_rect.x = pos_x - 20;
    dest_rect.y = pos_y - 20;
    dest_rect.h = 80;
    dest_rect.w = 80;

    collision_rect.x = 0;
    collision_rect.y = 0;
    collision_rect.h = 0;
    collision_rect.w = 0;

    update(0);
}

void Tank::grantShield() {
    shieldFlag = 1;
    shieldFrame = 0;
    if(shield  == nullptr) shield = new Object(pos_x, pos_y, SHIELD);
    shieldTime = 0;
}
