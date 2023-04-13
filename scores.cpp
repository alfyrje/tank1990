#include "scores.h"
#include "menu.h"
#include "playing.h"
#include "gameconfig.h"
#include "renderer.h"

#include<iostream>

Scores::Scores()
{
    showTime = 0;
    currentLevel = 0;
    gameOver = true;
    maxScore = 0;
    scoreCounter = 0;
    scoreCounterChange = true;
}

Scores::Scores(std::vector<Player *> t_players, int level, bool game_over) {
    players = t_players;
    currentLevel = level;
    gameOver = game_over;
    showTime = 0;
    maxScore = 0;
    scoreCounter = 0;
    scoreCounterChange = true;
    for(auto player : players) {
        player->to_erase = false;
        if(player->lives == 0 && !gameOver) player->lives = 2;
        else player->lives++;
        player->respawn();
        player->menuFlag = true;

        if(player->score > maxScore) maxScore = player->score;
    }
}

void Scores::draw() {
    Renderer& renderer = renderer.getRenderer();

    SDL_Point p_dst;
    SDL_Rect dst;

    p_dst = {200, 190};
    renderer.drawText(&p_dst, "STAGE " + std::to_string(currentLevel), {255, 255, 220, 255}, 1);
    p_dst = {175, 230};
    renderer.drawText(&p_dst, "PLAYER", {255, 255, 255, 255}, 2);
    p_dst = {345, 230};
    renderer.drawText(&p_dst, "SCORE", {255, 255, 255, 255}, 2);
    dst = {145, 255, 300, 2};
    SDL_SetRenderDrawColor(renderer.gameRenderer, 250, 250, 200, 255);
    SDL_RenderFillRect(renderer.gameRenderer, &dst);
    SDL_SetRenderDrawColor(renderer.gameRenderer, 0, 0, 0, 0);
    int i = 0;
    for(auto player : players) {
        dst = {175, 270 + i * (player->src_rect.h), player->src_rect.w, player->src_rect.h};
        renderer.drawObject(&player->src_rect, &dst);
        p_dst = {215, 278 + i * (player->src_rect.h)};
        renderer.drawText(&p_dst, "x" + std::to_string(player->lives), {255, 255, 255, 255}, 2);
        p_dst = {345, 278 + i * (player->src_rect.h)};
        renderer.drawText(&p_dst, (scoreCounter < player->score ? std::to_string(scoreCounter) : std::to_string(player->score)), {255, 255, 255, 255}, 2);
        i++;
    }


    renderer.flush();
}

void Scores::update(int dt) {

}

void Scores::eventProcess(SDL_Event *event) {

}

bool Scores::finished() const {
    return showTime > GameConfig::scores_show_time;
}

AppState *Scores::nextState() {
    if(gameOver) {
        Menu* m = new Menu;
        return m;
    }
    Playing* p = new Playing(players, currentLevel);
    return p;
}
