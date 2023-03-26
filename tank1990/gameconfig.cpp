#include "gameconfig.h"

std::string GameConfig::texture_path = "texture.png";
SDL_Rect GameConfig::map_rect = {0, 0, GameConfig::tile_rect.w * 26, GameConfig::tile_rect.h * 26};
SDL_Rect GameConfig::status_rect = {GameConfig::tile_rect.w * 26, 0, GameConfig::tile_rect.w * 3, GameConfig::map_rect.h};
SDL_Rect GameConfig::window_rect = {0, 0, GameConfig::map_rect.w + GameConfig::status_rect.w, GameConfig::map_rect.h};
SDL_Rect GameConfig::tile_rect = {0, 0, 20, 20};
double GameConfig::tank_default_speed = 0.08;
int GameConfig::reload_time = 120;
int GameConfig::max_bullet = 2;
double GameConfig::bullet_default_speed = 0.23;
