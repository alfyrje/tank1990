#ifndef appstate_h
#define appstate_h

#include<SDL.h>

class AppState {
public:
    virtual ~AppState() {}
    virtual bool finished() const = 0;
    virtual void draw() = 0;
    virtual void update(int dt) = 0;
    virtual void eventProcess(SDL_Event* ev) = 0;
    virtual AppState* nextState() = 0;
};
#endif
