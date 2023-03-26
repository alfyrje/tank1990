#include "playing.h"
#include "menu.h"
#include "renderer.h"
#include "main.h"

#include<iostream>

Playing::Playing() {
    player = new Player;
    playingFinished = false;
}

Playing::~Playing() {

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
    for(auto bullet : player->bullets) bullet->update(dt);
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
