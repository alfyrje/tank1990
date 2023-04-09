#include<bits/stdc++.h>
using namespace std;

#include<SDL.h>
#include<SDL_image.h>

int main(int argc, char* args[]) {

	SDL_Window* gameWindow = nullptr;
	SDL_Renderer* gameRenderer = nullptr;
	SDL_Texture* gameTexture = nullptr;

	if(SDL_Init(SDL_INIT_VIDEO) == 0) {
        IMG_Init(IMG_INIT_PNG);

        gameWindow = SDL_CreateWindow("Tank 199 fdsfdsafas 0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       20 * 26 + 20 * 3, 20 * 26, SDL_WINDOW_SHOWN);

		gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
    	gameTexture = IMG_LoadTexture(gameRenderer, "texture.png");
		SDL_Rect a = {0,0,20 * 26 + 20 * 3,20 * 26}, b = {16,16,16,16};
		SDL_RenderCopy(gameRenderer, gameTexture, NULL, NULL);
		SDL_RenderPresent(gameRenderer);
		SDL_Delay(3000);
        /*double FPS;
        int time1, time2, dt, fps_time = 0, fps_count = 0, delay = 15;
        time1 = clock();

        while(1) {

            SDL_RenderCopy(gameRenderer, gameTexture, &a, &b);
            //game.mainRenderer.drawObject(&a, &b);
            time2 = clock();
            dt = time2 - time1;
            time1 = time2;
            SDL_Delay(delay);
            fps_time += dt; fps_count++;
            if(fps_time > 200) {
                FPS = (double)fps_count / fps_time * 1000;
                if(FPS > 60) delay++;
                else if(delay > 0) delay--;
                fps_time = 0; fps_count = 0;
            }
        }
		*/
    }

    SDL_DestroyWindow(gameWindow);
    gameWindow = nullptr;
    IMG_Quit();
    SDL_Quit();

	return 0;
}
