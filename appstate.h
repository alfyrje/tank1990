#ifndef appstate_h
#define appstate_h

#include "renderer.h"
#include<SDL.h>

class AppState {
public:
    virtual ~AppState() {}
    virtual bool finished() const = 0;
    virtual void draw(Renderer* renderer) = 0;
    virtual void update(int dt) = 0;
    virtual void eventProcess(SDL_Event* ev) = 0;
    virtual AppState* nextState() = 0;
    SDL_Point p_dst;
    SDL_Rect dst;
    SDL_Rect src;
    int i;
};
#endif
