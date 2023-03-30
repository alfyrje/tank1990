#ifndef playing_h
#define playing_h

#include<SDL.h>
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

    SDL_Rect intersectRect(SDL_Rect *rect1, SDL_Rect *rect2);
    void checkCollisionTankWithLevel(Tank* tank, int dt);
    void checkCollisionTwoTanks(Tank* tank1, Tank* tank2, int dt);
    void checkCollisionBulletWithLevel(Bullet* bullet);
};

#endif
