#include <stdlib.h>

#include <stdio.h>

#include <SDL.h>

// draw a rectangle
int x=0;
bool xx=false;
int y=0;
bool yy=false;
int speed=1;
int rw=100 ,rh=50;

void drawRect(SDL_Renderer *renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = { x, y, rw, rh };
    SDL_RenderFillRect(renderer, &rect);
}

//make a window for a nintendo switch

int main(int argc, char *argv[]){
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;

    int done = 0, x = 0, w = 1920, h = 1080;

    // mandatory at least on switch, else gfx is not properly closed
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        SDL_Log("SDL_Init: %s\n", SDL_GetError());
        return -1;
    }
    
    // create an SDL window (OpenGL ES2 always enabled)
    // when SDL_FULLSCREEN flag is not set, viewport is automatically handled by SDL (use SDL_SetWindowSize to "change resolution")
    // available switch SDL2 video modes :
    // 1920 x 1080 @ 32 bpp (SDL_PIXELFORMAT_RGBA8888)
    // 1280 x 720 @ 32 bpp (SDL_PIXELFORMAT_RGBA8888)
    window = SDL_CreateWindow("sdl2_gles2", 0, 0, 1920, 1080, 0);
    if (!window) {
        SDL_Log("SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    for (int i = 0; i < 2; i++) {
        if (SDL_JoystickOpen(i) == NULL) {
            SDL_Log("SDL_JoystickOpen: %s\n", SDL_GetError());
            SDL_Quit();
            return -1;
        }
    }

    while (!done) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_JOYAXISMOTION:
                    SDL_Log("Joystick %d axis %d value: %d\n",
                            event.jaxis.which,
                            event.jaxis.axis, event.jaxis.value);
                    break;

                case SDL_JOYBUTTONDOWN:
                    SDL_Log("Joystick %d button %d down\n",
                            event.jbutton.which, event.jbutton.button);
                    if (event.jbutton.which == 0) {
                        if (event.jbutton.button == 0)
                        {
                            speed++;
                        }else if (event.jbutton.button == 1)
                        {
                            speed--;
                        }
                        if (event.jbutton.button == 12)
                        {
                            rw-=10;
                        }else if (event.jbutton.button == 13)
                        {
                            rh-=10;
                        }else if (event.jbutton.button == 14)
                        {
                            rw+=10;
                        }else if(event.jbutton.button == 15)
                        {
                            rh+=10;
                        }
                        
                        if (event.jbutton.button == 10) {
                            // (+) button down
                            done = 1;
                        }
                    }
                    break;

                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // fill widow bounds with black color
        SDL_SetRenderDrawColor(renderer, 111, 111, 111, 255);
        SDL_GetWindowSize(window, &w, &h);
        SDL_Rect f = {0, 0, w, h};
        SDL_RenderFillRect(renderer, &f);
        
        // make the rectangle move
        if(xx){
            x-=speed;
        }else{
            x+=speed;
        }
        if(yy){
            y-=speed;
        }else{
            y+=speed;
        }

        if(x > w-rw){
            xx=true;
            x=w-rw;
        }else if(x < 0){
            xx=false;
            x=0;
        }
        if(y > h-rh){
            yy=true;
            y=h-rh;
        }else if(y < 0){
            yy=false;
            y=0;
        }
        drawRect(renderer, x, y);

        SDL_RenderPresent(renderer);

    }
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}