#include "playing.h"
#include "menu.h"
#include "renderer.h"
#include "main.h"
#include "object.h"
#include "gameconfig.h"

#include<iostream>

Playing::Playing() {
    player = new Player;
    playingFinished = false;
}

Playing::~Playing() {

}

SDL_Rect Playing::intersectRect(SDL_Rect *rect1, SDL_Rect *rect2) {
    SDL_Rect intersect_rect;
    intersect_rect.x = std::max(rect1->x, rect2->x);
    intersect_rect.y = std::max(rect1->y, rect2->y);
    intersect_rect.w = std::min(rect1->x + rect1->w, rect2->x + rect2->w) - intersect_rect.x;
    intersect_rect.h = std::min(rect1->y + rect1->h, rect2->y + rect2->h) - intersect_rect.y;

    return intersect_rect;
}

void Playing::draw() {
    Renderer& renderer = Renderer::getRenderer();
    player->draw();
    for(auto bullet : player->bullets) bullet->draw();

    renderer.flush();
}

void Playing::update(int dt) {
    if(dt > 40) return;

    player->update(dt);
    checkCollisionTankWithLevel(player, dt);

    for(auto bullet : player->bullets) {
        checkCollisionBulletWithLevel(bullet);
        //checkCollisionBulletWithBush(bullet);
    }


}

void Playing::eventProcess(SDL_Event* ev) {

}

bool Playing::finished() const {
    return playingFinished;
}

AppState* Playing::nextState() {
    Menu* m = new Menu;
    return m;
}

void Playing::checkCollisionTankWithLevel(Tank* tank, int dt) {
    if(tank->to_erase) return;

    SDL_Rect pr, an;

    pr = tank->nextCollisionRect(dt);
    SDL_Rect intersect_rect;

    SDL_Rect outside_map_rect;

    outside_map_rect.x = -GameConfig::tile_rect.w;
    outside_map_rect.y = 0;
    outside_map_rect.w = GameConfig::tile_rect.w;
    outside_map_rect.h = GameConfig::map_rect.h;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);

    outside_map_rect.x = GameConfig::map_rect.w;
    outside_map_rect.y = 0;
    outside_map_rect.w = GameConfig::tile_rect.w;
    outside_map_rect.h = GameConfig::map_rect.h;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);

    outside_map_rect.x = 0;
    outside_map_rect.y = -GameConfig::tile_rect.h;
    outside_map_rect.w = GameConfig::map_rect.w;
    outside_map_rect.h = GameConfig::tile_rect.h;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);

    outside_map_rect.x = 0;
    outside_map_rect.y = GameConfig::map_rect.h;
    outside_map_rect.w = GameConfig::map_rect.w;
    outside_map_rect.h = GameConfig::tile_rect.h;
    intersect_rect = intersectRect(&outside_map_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);
}

void Playing::checkCollisionTwoTanks(Tank* tank1, Tank* tank2, int dt) {
    SDL_Rect pr1 = tank1->nextCollisionRect(dt);
    SDL_Rect pr2 = tank2->nextCollisionRect(dt);
    SDL_Rect intersect_rect = intersectRect(&pr1, &pr2);

    if(intersect_rect.w > 0 && intersect_rect.h > 0)
    {
        tank1->collide(intersect_rect);
        tank2->collide(intersect_rect);
    }
}

void Playing::checkCollisionBulletWithLevel(Bullet* bullet) {
    if(bullet == nullptr) return;
    if(bullet->collide) return;

    SDL_Rect *pr, *an;
    SDL_Rect intersect_rect;

    pr = &bullet->collision_rect;

    if(pr->x < 0 || pr->y < 0 || pr->x + pr->w > GameConfig::map_rect.w || pr->y + pr->h > GameConfig::map_rect.h) {
        bullet->destroy();
    }
}

