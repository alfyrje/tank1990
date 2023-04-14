#include "menu.h"
#include "playing.h"
#include "gameconfig.h"

#include<iostream>

Menu::Menu() {
    menuTexts.clear();
    menuTexts.push_back("1 Player");
    menuTexts.push_back("2 Players");
    menuTexts.push_back("Exit");
    menuIndex = 0;
    tankPointer = new Player(0);
    tankPointer->setDirection(1);
    tankPointer->pos_x = 180;
    tankPointer->pos_y = menuIndex * 40 + 175;
    tankPointer->Tank::update(0);
    menuFinished = false;
}

Menu::~Menu() {
    menuTexts.clear();
    delete tankPointer;
}

void Menu::draw(Renderer* renderer) {

    SDL_Rect logo = {0, 260, 406, 72};
    dst = {(GameConfig::map_rect.w + GameConfig::status_rect.w - 406) / 2, 50, 406, 72};
    renderer->drawObject(&logo, &dst);

    i = 0;
    SDL_Point text_start;
    for(auto text : menuTexts) {
        text_start = {225, i * 40 + 190};
        i++;
        renderer->drawText(&text_start, text, {255, 255, 255, 255}, 2);
    }
    tankPointer->draw(renderer);
    renderer->flush();
}

void Menu::update(int dt) {
    tankPointer->Tank::update(dt);
}

void Menu::eventProcess(SDL_Event* event) {
    if(event->type == SDL_KEYDOWN) {
        if(event->key.keysym.sym == SDLK_UP) {
            menuIndex--;
            if(menuIndex < 0)
                menuIndex = menuTexts.size() - 1;

            tankPointer->pos_y = menuIndex * 40 + 175;
        }
        else if(event->key.keysym.sym == SDLK_DOWN) {
            menuIndex++;
            if(menuIndex >= (int)menuTexts.size())
                menuIndex = 0;

            tankPointer->pos_y = menuIndex * 40 + 175;
        }
        else if(event->key.keysym.sym == SDLK_SPACE || event->key.keysym.sym == SDLK_RETURN) {
            menuFinished = true;
        }
        else if(event->key.keysym.sym == SDLK_ESCAPE) {
            menuIndex = -1;
            menuFinished = true;
        }
    }
}

bool Menu::finished() const {
    return menuFinished;
}

AppState* Menu::nextState() {
    if(menuIndex == (int)menuTexts.size() - 1) return nullptr;
    else if(menuIndex == 0) {
        Playing* p = new Playing(1);
        return p;
    }
    else if(menuIndex == 1) {
        Playing* p = new Playing(2);
        return p;
    }
    return nullptr;
}
