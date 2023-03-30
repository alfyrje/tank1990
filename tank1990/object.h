#ifndef object_h
#define object_h

#include "SDL.h"

class Object {
public:
    Object();
    virtual ~Object();
    virtual void draw();
    virtual void update(int dt);
    bool to_erase;
    SDL_Rect dest_rect;
    SDL_Rect collision_rect;
    SDL_Rect src_rect;
    double pos_x;
    double pos_y;
    SDL_Rect moveRect(const SDL_Rect &rect, int x, int y);
};



#endif
