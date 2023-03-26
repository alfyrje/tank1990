#ifndef renderer_h
#define renderer_h

#include <SDL.h>

class Renderer {
public:
    Renderer();
    ~Renderer();
    SDL_Renderer* gameRenderer;
    SDL_Texture* gameTexture;
    void loadTexture(SDL_Window* window);
    void drawObject(SDL_Rect* texture_src, SDL_Rect *window_dest);
    void flush();
    static Renderer& getRenderer();
};

//static Renderer mainRenderer;

#endif
