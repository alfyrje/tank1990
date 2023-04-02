#ifndef gameconfig_h
#define gameconfig_h

#include<SDL.h>
#include<string>

class GameConfig {
public:
    static SDL_Rect window_rect;
    static SDL_Rect map_rect;
    static SDL_Rect status_rect;
    static SDL_Rect tile_rect;
    static std::string texture_path;
    static double tank_default_speed;
    static double bullet_default_speed;
    static int reload_time;
    static int max_bullet;
    static std::string levels_path;
    static int enemy_start_count;
    static std::string font_name;
};

#endif

