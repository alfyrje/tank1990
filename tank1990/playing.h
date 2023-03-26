#ifndef playing_h
#define playing_h

#include "appstate.h"
#include "player.h"

class Playing : public AppState {
public:
    Playing();
    ~Playing();
    bool finished() const;
    void draw();
    void update(int dt);
    void eventProcess(SDL_Event* ev);
    AppState* nextState();
    Player* player;
    bool playingFinished;
};

#endif
