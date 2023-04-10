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
    spawnFlag = 0;
    direction = 0;
    src_rect = {640, 0, 32, 32};
    dest_rect = {160, 480, 40, 40};
    pos_x = 160;
    pos_y = 480;
    speed = 0;
    fireTime = 0;
    maxBullet = GameConfig::max_bullet;
    stop = 0;
    src_x = 24 * 16;
    to_erase = false;
    destroyFlag = false;
    shieldFlag = 0;
    shield = nullptr;
}

Player::Player(bool isPlayer2) {
    lives = 11;
    stars = 0;
    spawnFlag = 0;
    direction = 0;
    if(!isPlayer2) {
        src_rect = {640, 0, 32, 32};
        dest_rect = {160, 480, 40, 40};
        pos_x = 160;
        pos_y = 480;
        src_x = 640;
        playerKeys.up = SDL_SCANCODE_UP;
        playerKeys.down = SDL_SCANCODE_DOWN;
        playerKeys.left = SDL_SCANCODE_LEFT;
        playerKeys.right = SDL_SCANCODE_RIGHT;
        playerKeys.fire = SDL_SCANCODE_SLASH;
        player2 = 0;
    }
    else {
        src_rect = {768, 0, 32, 32};
        dest_rect = {320, 480, 40, 40};
        pos_x = 320;
        pos_y = 480;
        src_x = 768;
        playerKeys.up = SDL_SCANCODE_W;
        playerKeys.down = SDL_SCANCODE_S;
        playerKeys.left = SDL_SCANCODE_A;
        playerKeys.right = SDL_SCANCODE_D;
        playerKeys.fire = SDL_SCANCODE_LCTRL;
        player2 = 1;
    }
    speed = 0;
    fireTime = 0;
    maxBullet = GameConfig::max_bullet;
    stop = 0;
    to_erase = false;
    shieldFlag = 0;
    shield = nullptr;
}


void Player::update(int dt) {
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    Tank::update(dt);

    defaultSpeed = GameConfig::tank_default_speed;

    if (keyState[playerKeys.left]) { setDirection(3); speed = defaultSpeed; }
    else if (keyState[playerKeys.right]) { setDirection(1); speed = defaultSpeed; }
    else if (keyState[playerKeys.up]) { setDirection(0); speed = defaultSpeed; }
    else if (keyState[playerKeys.down]) { setDirection(2); speed = defaultSpeed; }
    else {
        speed = 0.0;
    }
    if (keyState[playerKeys.fire] && fireTime > GameConfig::reload_time) {
        fire();
        fireTime = 0;
    }

    fireTime += dt;

    src_y = stars * 2 * 32;

    stop = false;
}

Bullet* Player::fire() {
    Bullet* b = Tank::fire();
    if(b != nullptr) {
        if(stars > 0) b->speed = GameConfig::bullet_default_speed * 1.3;
        if(stars == 3) b->increased_damage = true;
    }
    return b;
}

void Player::respawn() {
    lives--;
    if(lives <= 0) {
        if(bullets.empty()) to_erase = true;
        return;
    }

    if(!player2) {
        pos_x = 160;
        pos_y = 480;
    }
    else {
        pos_x = 320;
        pos_y = 480;
    }

    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
    dest_rect.h = 40;
    dest_rect.w = 40;

    setDirection(0);

    Tank::respawn();
    grantShield();
    shieldTime = GameConfig::tank_shield_time / 2;
    shieldFrame = 5000 / 100;

    maxBullet = GameConfig::max_bullet;
}

void Player::destroy() {
    if(shieldFlag || destroyFlag) return;

    if(stars == 3) changeStarCountBy(-1);
    else {
        changeStarCountBy(-3);
        Tank::destroy();
    }
}

void Player::changeStarCountBy(int c) {
    stars += c;
    if(stars > 3) stars = 3;
    else if(stars < 0) stars = 0;

    if(stars >= 2 && c > 0) maxBullet++;
    else maxBullet = 2;

    if(stars > 0) defaultSpeed = GameConfig::tank_default_speed * 1.3;
    else defaultSpeed = GameConfig::tank_default_speed;
}
