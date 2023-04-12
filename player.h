#ifndef player_h
#define player_h

#include "tank.h"
#include<SDL.h>

class Player : public Tank {
public:
    struct player_key {
        SDL_Scancode up;
        SDL_Scancode right;
        SDL_Scancode down;
        SDL_Scancode left;
        SDL_Scancode fire;
    };
    player_key playerKeys;
    Player();
    Player(bool isPlayer2);

    int stars;
    int score;
    bool player2;
    void update(int dt);
    Bullet* fire();
    void respawn();
    void destroy();
    void changeStarCountBy(int c);
};

#endif
