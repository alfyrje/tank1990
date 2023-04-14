#ifndef object_h
#define object_h

#include "renderer.h"
#include "SDL.h"

enum OBJECT_TYPE {
    BRICK_WALL,
    STONE_WALL,
    BUSH,
    TANK_A,
    TANK_B,
    TANK_C,
    TANK_D,
    SHIELD,
    BONUS_STAR,
    BONUS_GRENADE,
    BONUS_CLOCK,
    BONUS_TANK,
    BONUS_GUN,
    BONUS_HELMET,
};

class Object {
public:
    Object();
    Object(double x, double y, OBJECT_TYPE type);
    virtual ~Object();
    virtual void draw(Renderer* renderer);
    virtual void update(int dt);
    bool to_erase;
    SDL_Rect dest_rect;
    SDL_Rect collision_rect;
    SDL_Rect src_rect;
    SDL_Rect pre_src_rect;
    double pos_x;
    double pos_y;
    OBJECT_TYPE TYPE;
    SDL_Rect moveRect(const SDL_Rect &rect, int x, int y);
    int destroyFlag;
    int destroyTime;
    int destroyFrame;
    int src_x;
    int src_y;
    SDL_Point p_dst;
    SDL_Rect dst;
};



#endif
