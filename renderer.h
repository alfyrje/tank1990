#ifndef renderer_h
#define renderer_h

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Renderer {
public:
    Renderer();
    ~Renderer();
    SDL_Renderer* gameRenderer;
    SDL_Texture* gameTexture;
    SDL_Texture* textTexture;
    SDL_Surface* text_surface;
    void loadTexture(SDL_Window* window);
    void drawObject(SDL_Rect* texture_src, SDL_Rect *window_dest);
    void drawText(const SDL_Point* start, std::string text, SDL_Color text_color, int font_size);
    void flush();
    void loadFont();
    TTF_Font* font1;
    TTF_Font* font2;
    TTF_Font* font3;
};

//static Renderer mainRenderer;

#endif
