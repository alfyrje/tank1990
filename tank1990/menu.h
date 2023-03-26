#ifndef menu_h
#define menu_h

#include "appstate.h"

class Menu : public AppState {
public:
    Menu();
    ~Menu();
    bool finished() const;
    void draw();
    void update(int dt);
    void eventProcess(SDL_Event* ev);
    AppState* nextState();
    bool menuFinished;
};

#endif

