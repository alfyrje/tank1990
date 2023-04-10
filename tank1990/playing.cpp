#include "playing.h"
#include "menu.h"
#include "renderer.h"
#include "main.h"
#include "object.h"
#include "gameconfig.h"
#include "brick.h"
#include "game.h"

#include<chrono>
#include<SDL.h>
#include<fstream>
#include<iostream>
#include<string>
#include<SDL_image.h>
#include<algorithm>

Playing::Playing() {
    playersCount = 1;
    playingFinished = false;
    levelColumns = 0;
    levelRows = 0;
    currentLevel = 0;
    enemyReadyTime = 0;
    enemySpawnPos = 0;
    nextLevel();
}

Playing::Playing(int players_count) {
    playersCount = players_count;
    playingFinished = false;
    levelColumns = 0;
    enemySpawnPos = 0;
    levelRows = 0;
    currentLevel = 0;
    enemyReadyTime = 0;
    nextLevel();
}

Playing::~Playing() {
    for(auto player : players) delete player;
    players.clear();

    for(auto enemy : enemies) delete enemy;
    enemies.clear();

    for(auto row : level) {
        for(auto item : row) if(item != nullptr) delete item;
        row.clear();
    }
    level.clear();

    if(eagle != nullptr) delete eagle;
    eagle = nullptr;
}

void Playing::nextLevel() {
    currentLevel++;

    gameOver = false;
    playingFinished = false;
    enemyToKill = GameConfig::enemy_start_count;

    std::string level_path = GameConfig::levels_path + std::to_string(currentLevel) + ".txt";
    loadLevel(level_path);

    if(players.empty()) {
        if(playersCount == 2) {
            Player* p1 = new Player(0);
            Player* p2 = new Player(1);
            players.push_back(p1);
            players.push_back(p2);

        }
        else
        {
            Player* p1 = new Player(0);
            players.push_back(p1);
        }
    }
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
    for(auto player : players) player->draw();
    for(auto enemy : enemies) enemy->draw();
    for(auto bush : bushes) bush->draw();
    //for(auto bonus : bonuses) bonus->draw();
    eagle->draw();

    if(gameOver) {
        SDL_Point pos;
        pos.x = 140;
        pos.y = gameOverTextPos;
        renderer.drawText(&pos, GameConfig::game_over_text, {255, 10, 10, 255}, 1);
    }

    SDL_Rect src = {944, 144, 16, 16};
    SDL_Rect dst;
    SDL_Point p_dst;

    for(int i = 0; i < enemyToKill; i++) {
        dst = {GameConfig::status_rect.x + 8 + src.w * (i % 2), 5 + src.h * (i / 2), src.w, src.h};
        renderer.drawObject(&src, &dst);
    }

    int i = 0;
    for(auto player : players) {
        dst = {GameConfig::status_rect.x + 5, i * 18 + 180, 16, 16};
        p_dst = {dst.x + dst.w + 2, dst.y + 3};
        renderer.drawObject(&player->src_rect, &dst);
        renderer.drawText(&p_dst, std::to_string(player->lives), {0, 0, 0, 255}, 3);
        i++;
    }

    src = {976, 64, 32, 32};
    dst = {GameConfig::status_rect.x + 8, 185 + playersCount * 18, src.w, src.h};
    p_dst = {dst.x + 10, dst.y + 26};
    renderer.drawObject(&src, &dst);
    renderer.drawText(&p_dst, std::to_string(currentLevel), {0, 0, 0, 255}, 2);


    renderer.flush();
}

void Playing::update(int dt) {
    if(dt > 40) return;

    std::vector<Player*>::iterator pl1, pl2;
    std::vector<Enemy*>::iterator en1, en2;

    for(auto enemy : enemies)
        for(auto bullet : enemy->bullets)
            checkCollisionBulletWithLevel(bullet);
    for(auto player : players)
        for(auto bullet : player->bullets) {
            checkCollisionBulletWithLevel(bullet);
            checkCollisionBulletWithBush(bullet);
        }

    for(auto player : players) {
        for(auto enemy : enemies) {
            checkCollisionTwoTanks(player, enemy, dt);
            checkCollisionPlayerBulletsWithEnemy(player, enemy);

            for(auto bullet1 : player->bullets) {
                for(auto bullet2 : enemy->bullets) {
                    checkCollisionTwoBullets(bullet1, bullet2);
                }
            }
        }
    }

    for(auto enemy : enemies)
        for(auto player : players)
                checkCollisionEnemyBulletsWithPlayer(enemy, player);

    for(pl1 = players.begin(); pl1 != players.end(); pl1++)
        for(pl2 = pl1 + 1; pl2 != players.end(); pl2++)
            checkCollisionTwoTanks(*pl1, *pl2, dt);

    for(en1 = enemies.begin(); en1 != enemies.end(); en1++)
        for(en2 = en1 + 1; en2 != enemies.end(); en2++)
            checkCollisionTwoTanks(*en1, *en2, dt);

    for(auto player : players) checkCollisionTankWithLevel(player, dt);
    for(auto enemy : enemies) checkCollisionTankWithLevel(enemy, dt);

    int min_dist;
    int dist;
    SDL_Point target;

    for(auto enemy : enemies) {
        min_dist = 1000;
        if(enemy->TYPE == TANK_A || enemy->TYPE == TANK_D) {
            for(auto player : players) {
                dist = abs(player->dest_rect.x - enemy->dest_rect.x) + abs(player->dest_rect.y - enemy->dest_rect.y);
                if(min_dist > dist) {
                    min_dist = dist;
                    target = {player->dest_rect.x + player->dest_rect.w / 2, player->dest_rect.y + player->dest_rect.h / 2};
                }
            }
        }
        dist = abs(eagle->dest_rect.x - enemy->dest_rect.x) + abs(eagle->dest_rect.y - enemy->dest_rect.y);
        if(min_dist > dist) {
            min_dist = dist;
            target = {eagle->dest_rect.x + eagle->dest_rect.w / 2, eagle->dest_rect.y + eagle->dest_rect.h / 2};
        }
        enemy->targetPos = target;
    }


    for(auto player : players) player->update(dt);
    for(auto enemy : enemies) enemy->update(dt);
    for(auto bonus : bonuses) bonus->draw();
    eagle->update(dt);
    for(auto row : level) for(auto item : row) if(item != nullptr) item->update(dt);
    for(auto bush : bushes) bush->update(dt);

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy*e){if(e->to_erase || (e->destroyFrame >= 7)) {delete e; return true;} return false;}), enemies.end());
    players.erase(std::remove_if(players.begin(), players.end(), [&](Player*p){if(p->to_erase) {killedPlayers.push_back(p); return true;} return false;}), players.end());
    bushes.erase(std::remove_if(bushes.begin(), bushes.end(), [](Object*b){if(b->to_erase) {delete b; return true;} return false;}), bushes.end());
    bonuses.erase(std::remove_if(bonuses.begin(), bonuses.end(), [](Bonus*b){if(b->to_erase) {delete b; return true;} return false;}), bonuses.end());

    enemyReadyTime += dt;
    if((int)enemies.size() < std::min(GameConfig::max_enemy_on_map, enemyToKill) && enemyReadyTime > GameConfig::enemy_ready_time) {
        enemyReadyTime = 0;
        generateEnemy();
    }

    if(enemies.empty() && enemyToKill <= 0) {
        levelEndTime += dt;
        if(levelEndTime > GameConfig::level_end_time)
            playingFinished = true;
    }

    if(players.empty() && gameOver) {
        eagle->destroy();
        gameOverTextPos = GameConfig::map_rect.h;
        gameOver = true;
    }

    if(gameOver) {
        if(gameOverTextPos < GameConfig::map_rect.h / 2 - 10) playingFinished = true;
        else gameOverTextPos -= GameConfig::game_over_entry_speed * dt;
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

    intersect_rect = intersectRect(&eagle->collision_rect, &pr);
    if(intersect_rect.w > 0 && intersect_rect.h > 0)
        tank->collide(intersect_rect);
}

void Playing::checkCollisionTwoTanks(Tank* tank1, Tank* tank2, int dt) {
    SDL_Rect pr1 = tank1->nextCollisionRect(dt);
    SDL_Rect pr2 = tank2->nextCollisionRect(dt);
    SDL_Rect intersect_rect = intersectRect(&pr1, &pr2);

    if(intersect_rect.w > 0 && intersect_rect.h > 0) {
        tank1->collide(intersect_rect);
        tank2->collide(intersect_rect);
    }
}

void Playing::checkCollisionBulletWithBush(Bullet *bullet) {
    if(bullet == nullptr) return;
    if(bullet->collide) return;
    if(!bullet->increased_damage) return;

    SDL_Rect *pr, *an;
    SDL_Rect intersect_rect;
    pr = &bullet->collision_rect;

    for(auto bush : bushes) {
        if(bush->to_erase) continue;
        an = &bush->collision_rect;
        intersect_rect = intersectRect(pr, an);

        if(intersect_rect.w > 0 && intersect_rect.h > 0) {
            bullet->destroy();
            bush->to_erase = true;
        }
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

    if(eagle->type == 0 && !gameOver) {
        intersect_rect = intersectRect(&eagle->collision_rect, pr);
        if(intersect_rect.w > 0 && intersect_rect.h > 0)
        {
            bullet->destroy();
            eagle->destroy();
            gameOverTextPos = GameConfig::map_rect.h;
            gameOver = true;
        }
    }

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
            if(o == nullptr) return;
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


}

void Playing::checkCollisionTwoBullets(Bullet *bullet1, Bullet *bullet2) {
    if(bullet1 == nullptr || bullet2 == nullptr) return;
    if(bullet1->to_erase || bullet2->to_erase) return;

    SDL_Rect intersect_rect = intersectRect(&bullet1->collision_rect, &bullet2->collision_rect);

    if(intersect_rect.w > 0 && intersect_rect.h > 0) {
        bullet1->destroy();
        bullet2->destroy();
    }
}

void Playing::checkCollisionPlayerBulletsWithEnemy(Player* player, Enemy* enemy) {
    if(player->to_erase || enemy->to_erase) return;
    if(enemy->destroyFlag) return;
    SDL_Rect intersect_rect;

    for(auto bullet : player->bullets) {
        if(!bullet->to_erase && !bullet->collide) {
            intersect_rect = intersectRect(&bullet->collision_rect, &enemy->collision_rect);
            if(intersect_rect.w > 0 && intersect_rect.h > 0) {
                if(enemy->bonusFlag) generateBonus();
                bullet->destroy();
                enemy->destroy();
                if(enemy->lives == 0) {
                    enemyToKill--;
                    player->score += 100;
                }
                else player->score += 50;
            }
        }
    }
}

void Playing::checkCollisionEnemyBulletsWithPlayer(Enemy* enemy, Player* player) {
    if(enemy->to_erase || player->to_erase) return;
    if(player->destroyFlag) return;
    SDL_Rect intersect_rect;

    for(auto bullet : enemy->bullets) {
        if(!bullet->to_erase && !bullet->collide) {
            intersect_rect = intersectRect(&bullet->collision_rect, &player->collision_rect);
            if(intersect_rect.w > 0 && intersect_rect.h > 0) {
                bullet->destroy();
                player->destroy();
            }
        }
    }
}

void Playing::generateBonus() {
}

void Playing::generateEnemy() {
    OBJECT_TYPE type = (OBJECT_TYPE)(rand() % (TANK_D - TANK_A + 1) + TANK_A);
    Enemy* e;
    if(enemySpawnPos == 0) e = new Enemy(1, 1, type);
    else if(enemySpawnPos == 1) e = new Enemy(240, 1, type);
    else e = new Enemy(480, 1, type);
    enemySpawnPos = (enemySpawnPos + 1) % 3;

    double p = 1.0 * rand() / RAND_MAX;

    if(p < 0.12) {
        e->bonusFlag = true;
        e->lives = 1;
    }
    else {
        e->bonusFlag = false;
        p = 1.0 * rand() / RAND_MAX;
        if(p < 0.1) e->lives = 4;
        else if(p < 0.3) e->lives = 3;
        else if(p < 0.6) e->lives = 2;
        else e->lives = 1;
    }
    enemies.push_back(e);
}
