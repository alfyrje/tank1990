#include "bullet.h"
#include "renderer.h"

Bullet::Bullet() {
    to_erased = false;
    increased_damage = false;
    speed = 0.0;
    direction = 0;
}

Bullet::Bullet(double x, double y) {
    to_erased = false;
    increased_damage = false;
    speed = 0.0;
    direction = 0;
    pos_x = x; pos_y = y;
    dest_rect.x = x;
    dest_rect.y = y;
    dest_rect.w = 10;
    dest_rect.h = 10;
    src_rect = {944, 128, 8, 8};
}

Bullet::~Bullet() {

}

void Bullet::update(int dt) {
    if(!to_erased) {
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
    }
}

void Bullet::draw() {
    Renderer& renderer = Renderer::getRenderer();
    renderer.drawObject(&src_rect, &dest_rect);
}
