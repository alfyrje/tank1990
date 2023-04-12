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
}

Scores::Scores(std::vector<Player *> t_players, int level, bool game_over) {
    players = t_players;
    currentLevel = level;
    gameOver = game_over;
    showTime = 0;
    maxScore = 0;
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
