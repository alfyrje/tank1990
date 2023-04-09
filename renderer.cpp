#include "renderer.h"
#include "gameconfig.h"
#include "main.h"

#include<bits/stdc++.h>
#include<SDL_image.h>
#include<SDL.h>

Renderer::Renderer() {
    gameRenderer = nullptr;
}

Renderer::~Renderer() {
    if(gameRenderer != nullptr) SDL_DestroyRenderer(gameRenderer);
}

void Renderer::loadTexture(SDL_Window* window) {
    gameRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    gameTexture = IMG_LoadTexture(gameRenderer, GameConfig::texture_path.c_str());
}

void Renderer::loadFont() {
    font1 = TTF_OpenFont(GameConfig::font_name.c_str(), 28);
    font2 = TTF_OpenFont(GameConfig::font_name.c_str(), 14);
    font3 = TTF_OpenFont(GameConfig::font_name.c_str(), 10);
}

void Renderer::drawObject(SDL_Rect *texture_src, SDL_Rect *window_dest) {
    SDL_RenderCopy(gameRenderer, gameTexture, texture_src, window_dest);
}

void Renderer::flush() {
    SDL_RenderPresent(gameRenderer);
    SDL_RenderClear(gameRenderer);
}

void Renderer::drawText(const SDL_Point* start, std::string text, SDL_Color text_color, int font_size) {
    if(font1 == nullptr || font2 == nullptr || font3 == nullptr) return;
    if(textTexture != nullptr) SDL_DestroyTexture(textTexture);

    SDL_Surface* text_surface = nullptr;
    if(font_size == 2) text_surface = TTF_RenderText_Solid(font2, text.c_str(), text_color);
    else if(font_size == 3) text_surface = TTF_RenderText_Solid(font3, text.c_str(), text_color);
    else text_surface = TTF_RenderText_Solid(font1, text.c_str(), text_color);

    textTexture = SDL_CreateTextureFromSurface(gameRenderer, text_surface);

    SDL_Rect window_dest;
    if(start == nullptr) {
        window_dest.x = (GameConfig::map_rect.w + GameConfig::status_rect.w - text_surface->w)/2;
        window_dest.y = (GameConfig::map_rect.h - text_surface->h)/2;
    }
    else {
        window_dest.x = start->x;
        window_dest.y = start->y;
    }
    window_dest.w = text_surface->w;
    window_dest.h = text_surface->h;

    SDL_RenderCopy(gameRenderer, textTexture, NULL, &window_dest);
}

Renderer& Renderer::getRenderer() {
    static Renderer renderer;
    return renderer;
}
