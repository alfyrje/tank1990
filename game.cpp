#include "game.h"
#include "gameconfig.h"
#include "renderer.h"
#include "main.h"
#include "playing.h"
#include "menu.h"

#include <bits/stdc++.h>
#include <chrono>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

Game::Game() {
    gameWindow = nullptr;
}

Game::~Game() {
    if(appState != nullptr) delete appState;
    if(renderer != nullptr) delete renderer;
}

void Game::run() {
    is_running = true;
    if(SDL_Init(SDL_INIT_VIDEO) == 0) {
        IMG_Init(IMG_INIT_PNG);
        TTF_Init();
        gameWindow = SDL_CreateWindow("Tank 1990", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       GameConfig::window_rect.w, GameConfig::window_rect.h, SDL_WINDOW_SHOWN);


        Renderer* renderer = new Renderer;
        renderer->loadTexture(gameWindow);
        renderer->loadFont();
        appState = new Menu;

        double FPS;
        int time1, time2, dt, fps_time = 0, fps_count = 0, delay = 15;
        time1 = clock();

        while(is_running) {
            time2 = clock();
            dt = time2 - time1;
            time1 = time2;

            if(appState->finished()) {
                AppState* newState = appState->nextState();
                delete appState;
                appState = newState;
            }
            if(appState == nullptr) break;

            Game::eventProcess();

            appState->update(dt);
            appState->draw(renderer);

            SDL_Delay(delay);
            fps_time += dt; fps_count++;
            if(fps_time > 200) {
                FPS = (double)fps_count / fps_time * 1000;
                if(FPS > 60) delay++;
                else if(delay > 0) delay--;
                fps_time = 0; fps_count = 0;
            }
        }
    }

    SDL_DestroyWindow(gameWindow);
    gameWindow = nullptr;
    IMG_Quit();
    SDL_Quit();
}

void Game::eventProcess() {
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT) {
        is_running = false;
    }
    appState->eventProcess(&event);
}
