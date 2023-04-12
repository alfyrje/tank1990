#ifndef gameconfig_h
#define gameconfig_h

#include<SDL.h>
#include<string>
#include<vector>

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
    static double game_over_entry_speed;
    static std::string game_over_text;
    static int max_enemy_on_map;
    static int enemy_ready_time;
    static int level_end_time;
    static int tank_shield_time;
    static int bonus_show_time;
    static int tank_frozen_time;
    static int level_start_time;
    static int scores_show_time;
};

#endif

