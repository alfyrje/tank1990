#include "playing.h"
#include "menu.h"
#include "renderer.h"
#include "main.h"
#include "object.h"
#include "gameconfig.h"
#include "brick.h"
#include "game.h"

#include<SDL.h>
#include<fstream>
#include<iostream>
#include<string>
#include<SDL_image.h>

Playing::Playing() {
    player = new Player;
    playingFinished = false;
    levelColumns = 0;
    levelRows = 0;
    currentLevel = 0;
    nextLevel();
}

Playing::~Playing() {

}

void Playing::nextLevel() {
    currentLevel++;

    gameOver = false;
    playingFinished = false;
    enemyToKill = GameConfig::enemy_start_count;

    std::string level_path = GameConfig::levels_path + std::to_string(currentLevel) + ".txt";
    loadLevel(level_path);
}

void Playing::loadLevel(std::string path) {
    std::ifstream level_path(path);
    std::string line;
    int j = -1;
    while(!level_path.eof()) {
        std::getline(level_path, line);
        std::vector<Object*> row;
        j++;
        for(unsigned i = 0; i < line.size(); i++) {
            Object* obj;
            switch(line[i]) {
            case '#' : obj = new Brick(i * GameConfig::tile_rect.w, j * GameConfig::tile_rect.h); break;
            case '@' : obj = new Object(i * GameConfig::tile_rect.w, j * GameConfig::tile_rect.h, STONE_WALL); break;
            case '%' : bushes.push_back(new Object(i * GameConfig::tile_rect.w, j * GameConfig::tile_rect.h, BUSH)); obj =  nullptr; break;
            default: obj = nullptr;
            }
            row.push_back(obj);
        }
        level.push_back(row);
    }
    levelRows = level.size();
    if(levelRows) levelColumns = level[0].size();
    else levelColumns = 0;

    eagle = new Eagle(12 * GameConfig::tile_rect.w, (levelRows - 2) * GameConfig::tile_rect.h);

    for(int i = 12; i < 14 && i < levelColumns; i++) {
        for(int j = levelRows - 2; j < levelRows; j++)
        {
            if(level[j][i] != nullptr){
                delete level[j][i];
                level[j][i] = nullptr;
            }
        }
    }
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

    SDL_SetRenderDrawColor(renderer.gameRenderer, 211, 211, 211, 0);
    SDL_RenderFillRect(renderer.gameRenderer, &GameConfig::status_rect);
    SDL_SetRenderDrawColor(renderer.gameRenderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer.gameRenderer, &GameConfig::map_rect);

    for(auto row : level) for(auto item : row) if(item != nullptr) item->draw();
    player->draw();
    for(auto bush: bushes) bush->draw();
    eagle->draw();

    SDL_Rect src = {944, 144, 16, 16};
    SDL_Rect dst;
    SDL_Point p_dst;

    for(int i = 0; i < enemyToKill; i++) {
        dst = {GameConfig::status_rect.x + 8 + src.w * (i % 2), 5 + src.h * (i / 2), src.w, src.h};
        renderer.drawObject(&src, &dst);
    }

    dst = {GameConfig::status_rect.x + 5, 180, 16, 16};
    p_dst = {dst.x + dst.w + 2, dst.y + 3};
    renderer.drawObject(&player->src_rect, &dst);
    renderer.drawText(&p_dst, std::to_string(player->lives), {0, 0, 0, 255}, 3);

    src = {976, 64, 32, 32};
    dst = {GameConfig::status_rect.x + 8, 203, src.w, src.h};
    p_dst = {dst.x + 10, dst.y + 26};
    renderer.drawObject(&src, &dst);
    renderer.drawText(&p_dst, std::to_string(currentLevel), {0, 0, 0, 255}, 2);

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

    SDL_Rect pr, *an;
    Object* o;

    int row_start, row_end;
    int column_start, column_end;

    pr = tank->nextCollisionRect(dt);
    SDL_Rect intersect_rect;


    switch(tank->direction) {
    case 0:
        row_end = tank->collision_rect.y / GameConfig::tile_rect.h;
        row_start = row_end - 1;
        column_start = tank->collision_rect.x / GameConfig::tile_rect.w - 1;
        column_end = (tank->collision_rect.x + tank->collision_rect.w) / GameConfig::tile_rect.w + 1;
        break;
    case 1:
        column_start = (tank->collision_rect.x + tank->collision_rect.w) / GameConfig::tile_rect.w;
        column_end = column_start + 1;
        row_start = tank->collision_rect.y / GameConfig::tile_rect.h - 1;
        row_end = (tank->collision_rect.y + tank->collision_rect.h) / GameConfig::tile_rect.h + 1;
        break;
    case 2:
        row_start = (tank->collision_rect.y + tank->collision_rect.h)/ GameConfig::tile_rect.h;
        row_end = row_start + 1;
        column_start = tank->collision_rect.x / GameConfig::tile_rect.w - 1;
        column_end = (tank->collision_rect.x + tank->collision_rect.w) / GameConfig::tile_rect.w + 1;
        break;
    case 3:
        column_end = tank->collision_rect.x / GameConfig::tile_rect.w;
        column_start = column_end - 1;
        row_start = tank->collision_rect.y / GameConfig::tile_rect.h - 1;
        row_end = (tank->collision_rect.y + tank->collision_rect.h) / GameConfig::tile_rect.h + 1;
        break;
    }
    if(column_start < 0) column_start = 0;
    if(row_start < 0) row_start = 0;
    if(column_end >= levelColumns) column_end = levelColumns - 1;
    if(row_end >= levelRows) row_end = levelRows - 1;

    for(int i = row_start; i <= row_end; i++) {
        for(int j = column_start; j <= column_end ;j++) {
            if(tank->stop) break;
            o = level[i][j];
            if(o == nullptr) continue;

            an = &o->collision_rect;

            intersect_rect = intersectRect(an, &pr);
            if(intersect_rect.w > 0 && intersect_rect.h > 0) tank->collide(intersect_rect);
        }
    }

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

    int row_start, row_end;
    int column_start, column_end;

    SDL_Rect *pr, *an;
    SDL_Rect intersect_rect;
    Object* o;

    pr = &bullet->collision_rect;

    switch(bullet->direction) {
    case 0:
        row_start = row_end = bullet->collision_rect.y / GameConfig::tile_rect.h;
        column_start = bullet->collision_rect.x / GameConfig::tile_rect.w;
        column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / GameConfig::tile_rect.w;
        break;
    case 1:
        column_start = column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / GameConfig::tile_rect.w;
        row_start = bullet->collision_rect.y / GameConfig::tile_rect.h;
        row_end = (bullet->collision_rect.y + bullet->collision_rect.h) / GameConfig::tile_rect.h;
        break;
    case 2:
        row_start = row_end = (bullet->collision_rect.y + bullet->collision_rect.h)/ GameConfig::tile_rect.h;
        column_start = bullet->collision_rect.x / GameConfig::tile_rect.w;
        column_end = (bullet->collision_rect.x + bullet->collision_rect.w) / GameConfig::tile_rect.w;
        break;
    case 3:
        column_start = column_end = bullet->collision_rect.x / GameConfig::tile_rect.w;
        row_start = bullet->collision_rect.y / GameConfig::tile_rect.h;
        row_end = (bullet->collision_rect.y + bullet->collision_rect.h) / GameConfig::tile_rect.h;
        break;
    }
    if(column_start < 0) column_start = 0;
    if(row_start < 0) row_start = 0;
    if(column_end >= levelColumns) column_end = levelColumns - 1;
    if(row_end >= levelRows) row_end = levelRows - 1;

    for(int i = row_start; i <= row_end; i++) {
        for(int j = column_start; j <= column_end; j++) {
            o = level[i][j];
            if(o == nullptr) continue;

            an = &o->collision_rect;
            intersect_rect = intersectRect(pr, an);

            if(intersect_rect.w > 0 && intersect_rect.h > 0) {
                if(bullet->increased_damage) {
                    delete o;
                    level[i][j] = nullptr;
                }
                else if(o->TYPE == BRICK_WALL) {
                    Brick* brick = (Brick*)(o);
                    brick->bulletHit(bullet->direction);
                    if(brick->to_erase) {
                        delete brick;
                        level[i][j] = nullptr;
                    }
                }
                bullet->destroy();
            }
        }
    }

    if(pr->x < 0 || pr->y < 0 || pr->x + pr->w > GameConfig::map_rect.w || pr->y + pr->h > GameConfig::map_rect.h) {
        bullet->destroy();
    }

    if(eagle->type == 0 && !gameOver){
        intersect_rect = intersectRect(&eagle->collision_rect, pr);
        if(intersect_rect.w > 0 && intersect_rect.h > 0)
        {
            bullet->destroy();
            eagle->destroy();
            gameOver = true;
        }
    }
}
