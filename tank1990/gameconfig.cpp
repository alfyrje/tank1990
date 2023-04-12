#include "gameconfig.h"

std::string GameConfig::font_name = "prstartk.ttf";
std::string GameConfig::texture_path = "texture.png";
SDL_Rect GameConfig::map_rect = {0, 0, GameConfig::tile_rect.w * 26, GameConfig::tile_rect.h * 26};
SDL_Rect GameConfig::status_rect = {GameConfig::tile_rect.w * 26, 0, GameConfig::tile_rect.w * 3, GameConfig::map_rect.h};
SDL_Rect GameConfig::window_rect = {0, 0, GameConfig::map_rect.w + GameConfig::status_rect.w, GameConfig::map_rect.h};
SDL_Rect GameConfig::tile_rect = {0, 0, 20, 20};
double GameConfig::tank_default_speed = 0.08;
int GameConfig::reload_time = 120;
int GameConfig::max_bullet = 2;
double GameConfig::bullet_default_speed = 0.23;
std::string GameConfig::levels_path = "levels/";
int GameConfig::enemy_start_count = 20;
double GameConfig::game_over_entry_speed = 0.13;
std::string GameConfig::game_over_text = "GAME OVER";
int GameConfig::max_enemy_on_map = 4;
int GameConfig::enemy_ready_time = 500;
int GameConfig::level_end_time = 5000;
int GameConfig::tank_shield_time = 10000;
int GameConfig::bonus_show_time = 10000;
int GameConfig::tank_frozen_time = 8000;
