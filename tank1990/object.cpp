#include "object.h"
#include "renderer.h"

#include<algorithm>
#include<SDL.h>


Object::Object() {
    pos_x = 0.0;
    pos_y = 0.0;
    to_erase = false;
}

Object::~Object() {

}

void Object::draw() {
    if(to_erase) return;
    Renderer& renderer = Renderer::getRenderer();
    renderer.drawObject(&src_rect, &dest_rect);
}

void Object::update(int dt) {
    if(to_erase) return;

    dest_rect.x = pos_x;
    dest_rect.y = pos_y;
}

SDL_Rect Object::moveRect(const SDL_Rect &rect, int x, int y) {
    SDL_Rect r;
    r.x = rect.x + x*rect.w;
    r.y = rect.y + y*rect.h;
    r.w = rect.w;
    r.h = rect.h;

    return r;
}
