#ifndef menu_h
#define menu_h

#include<vector>
#include "player.h"
#include "appstate.h"

class Menu : public AppState {
public:
    Menu();
    ~Menu();
    bool finished() const;
    void draw(Renderer* renderer);
    void update(int dt);
    void eventProcess(SDL_Event* event);
    AppState* nextState();
    bool menuFinished;
    std::vector<std::string> menuTexts;
    int menuIndex;
    Player* tankPointer;

};

#endif

