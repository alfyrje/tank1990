#ifndef game_h
#define game_h

#include "appstate.h"
#include<SDL.h>
#include "renderer.h"

class Game {
public:
    Game();
    ~Game();
    void run();
    SDL_Window* gameWindow;
    bool is_running;
    void eventProcess();
    AppState* appState;
    Renderer* renderer;
};

#endif
