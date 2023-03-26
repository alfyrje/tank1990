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
    gameTexture = IMG_LoadTexture(gameRenderer, "texture.png");
}

void Renderer::drawObject(SDL_Rect *texture_src, SDL_Rect *window_dest) {
    SDL_RenderCopy(gameRenderer, gameTexture, texture_src, window_dest);
}

void Renderer::flush() {
    SDL_RenderPresent(gameRenderer);
    SDL_RenderClear(gameRenderer);
}

Renderer& Renderer::getRenderer() {
    static Renderer renderer;
    return renderer;
}
