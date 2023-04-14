#include "enemy.h"
#include "gameconfig.h"

#include<iostream>

Enemy::Enemy() : Tank() {
    destroyFlag = false;
    speed = 0;
    fireTime = 0;
    bonusFlag = false;
    direction = 0;
    directionTime = 0;
    keepDirectionTime = 100;
    speedTime = 0;
    tryTime = 100;

    fireTime = 0;
    reloadTime = 100;
    lives = 1;

    maxBullet = 1;

    TYPE = TANK_A;

    if(TYPE == TANK_B)
        defaultSpeed = GameConfig::tank_default_speed * 1.3;
    else
        defaultSpeed = GameConfig::tank_default_speed;

    targetPos = {-1, -1};
    shieldFlag = 0;
    shield = nullptr;
    respawn();
}

Enemy::Enemy(double x, double y, OBJECT_TYPE type) : Tank() {
    destroyFlag = false;
    bonusFlag = false;
    speed = 0;
    fireTime = 0;
    pos_x = x;
    pos_y = y;
    dest_rect.x = x;
    dest_rect.y = y;
    dest_rect.w = 40;
    dest_rect.h = 40;
    direction = 0;
    directionTime = 0;
    keepDirectionTime = 100;

    speedTime = 0;
    tryTime = 100;

    fireTime = 0;
    reloadTime = 100;
    lives = 1;

    maxBullet = 1;

    TYPE = type;

    src_x = 128;
    src_rect.x = 128;
    src_rect.w = 32;
    src_rect.h = 32;

    if(TYPE == TANK_A) src_rect.y = 0;
    else if(TYPE == TANK_B) src_rect.y = 64;
    else if(TYPE == TANK_C) src_rect.y = 128;
    else if(TYPE == TANK_D) src_rect.y = 192;

    collision_rect = dest_rect;

    if(TYPE == TANK_B)
        defaultSpeed = GameConfig::tank_default_speed * 1.3;
    else
        defaultSpeed = GameConfig::tank_default_speed;

    targetPos = {-1, -1};
    shieldFlag = 0;
    shield = nullptr;

    respawn();
}

void Enemy::draw(Renderer* renderer) {
    if(to_erase) return;
    Tank::draw(renderer);
}

void Enemy::destroy() {
    lives--;
    if(lives <= 0) {
        lives = 0;
        if(!destroyFlag) Tank::destroy();
    }
}

void Enemy::update(int dt) {
    if(to_erase) return;
    Tank::update(dt);

    if(!destroyFlag && !spawnFlag) {
        if(bonusFlag) {
            src_rect.x = src_x + (direction - 4) * 32;
        }
        else {
            src_rect.x = src_x + (direction + (lives - 1) * 4) * 32;
        }
    }

    if(!destroyFlag && !spawnFlag) {
        directionTime += dt;
        speedTime += dt;
        fireTime += dt;
        if(!frozenFlag && directionTime > keepDirectionTime) {
            directionTime = 0;
            keepDirectionTime = rand() % 800 + 100;

            double p = 1.0 * rand() / RAND_MAX;
            if(p < 0.5) {
                int dx = targetPos.x - (dest_rect.x + dest_rect.w / 2);
                int dy = targetPos.y - (dest_rect.y + dest_rect.h / 2);

                p = 1.0 * rand() / RAND_MAX;
                if(abs(dx) > abs(dy))
                    setDirection(p < 0.6 ? (dx < 0 ? 3 : 1) : (dy < 0 ? 0 : 2));
                else
                    setDirection(p < 0.6 ? (dy < 0 ? 0 : 2) : (dx < 0 ? 3 : 1));
            }
            else
                setDirection((rand() % 4));

        }


        if(speedTime > tryTime) {
            speedTime = 0;
            tryTime = rand() % 300;
            speed = defaultSpeed;
        }

        if(fireTime > reloadTime) {
            fireTime = 0;
            if(TYPE == TANK_D) {
                reloadTime = rand() % 400 + 100;
                int dx = targetPos.x - (dest_rect.x + dest_rect.w / 2);
                int dy = targetPos.y - (dest_rect.y + dest_rect.h / 2);

                if(stop) fire();
                else switch (direction) {
                case 0:
                    if(dy < 0 && abs(dx) < dest_rect.w) fire();
                    break;
                case 1:
                    if(dx > 0 && abs(dy) < dest_rect.h) fire();
                    break;
                case 2:
                    if(dy > 0 && abs(dx) < dest_rect.w) fire();
                    break;
                case 3:
                    if(dx < 0 && abs(dy) < dest_rect.h) fire();
                    break;
                }
            }
            else if(TYPE == TANK_C) {
                reloadTime = rand() % 800 + 250;
                fire();
            }
            else {
                reloadTime = rand() % 1000 + 300;
                fire();
            }
        }
    }

    stop = false;
}
