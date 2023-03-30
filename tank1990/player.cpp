#include "player.h"
#include "renderer.h"
#include "main.h"
#include "gameconfig.h"
#include "bullet.h"

#include<algorithm>
#include<vector>
#include<iostream>

Player::Player() {
    lives = 11;
    stars = 0;
    direction = 0;
    src_rect = {24 * 16, 0, 32, 32};
    dest_rect = {160, 480, 40, 40};
    pos_x = 160;
    pos_y = 480;
    speed = 0;
    fireTime = 0;
    maxBullet = GameConfig::max_bullet;
    stop = 0;
    src_x = 24 * 16;
    to_erase = false;
}

Player::~Player() {
}

void Player::update(int dt) {
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    Tank::update(dt);

    //std::cout << dest_rect.x << ' ' << dest_rect.y << ' ' << stop << std::endl;
    if (keyState[SDL_SCANCODE_LEFT]) { setDirection(3); speed = GameConfig::tank_default_speed; }
    else if (keyState[SDL_SCANCODE_RIGHT]) { setDirection(1); speed = GameConfig::tank_default_speed; }
    else if (keyState[SDL_SCANCODE_UP]) { setDirection(0); speed = GameConfig::tank_default_speed; }
    else if (keyState[SDL_SCANCODE_DOWN]) { setDirection(2); speed = GameConfig::tank_default_speed; }
    else {
        speed = 0.0;
    }
    if (keyState[SDL_SCANCODE_C] && fireTime > GameConfig::reload_time) {
        fire();
        fireTime = 0;
    }

    fireTime += dt;

    stop = false;
}

Bullet* Player::fire() {
    Bullet* b = Tank::fire();
    if(b != nullptr) {
        if(stars > 0) b->speed = GameConfig::bullet_default_speed * 1.3;
        if(stars >= 3) b->increased_damage = true;
    }
    return b;
}
