#include "menu.h"
#include "playing.h"

Menu::Menu() {
    menuFinished = false;
}

Menu::~Menu() {

}

void Menu::draw() {

}

void Menu::update(int dt) {

}

void Menu::eventProcess(SDL_Event* ev) {

}

bool Menu::finished() const {
    return menuFinished;
}

AppState* Menu::nextState() {
    Playing* p = new Playing;
    return p;
}
