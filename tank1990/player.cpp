#include "player.h"
#include "renderer.h"
#include "main.h"
#include "gameconfig.h"
#include "bullet.h"
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
}

Player::~Player() {
    for(auto bullet : bullets) delete bullet;
}

void Player::update(int dt) {
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

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

    std::cout << dest_rect.x << ' ' << dest_rect.y << std::endl;
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
}

void Player::draw() {
    Renderer& renderer = Renderer::getRenderer();
    renderer.drawObject(&src_rect, &dest_rect);
}

void Player::setDirection(int d) {
    direction = d;
    src_rect.x = 24 * 16 + d * 32;
}

Bullet* Player::fire() {
    if(bullets.size() < GameConfig::max_bullet) {
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

        if(stars > 0) bullet->speed = bullet->speed * 1.3;
        if(stars >= 3) bullet->increased_damage = true;

        bullet->update(0);

        return bullet;
    }
    return nullptr;
}
