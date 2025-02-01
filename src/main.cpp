#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Surface* winSurface = NULL; 
    bool quit = false;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Error initializing SDL:" << SDL_GetError() << std::endl;
    }

    window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Failed to create a window! Error: " << SDL_GetError() << std::endl;
        system("pause");
        return -1;
    }

    winSurface = SDL_GetWindowSurface(window);
    
    if (!winSurface) {
        std::cerr << "Error getting surface! Error: " << SDL_GetError() << std::endl;
        system("pause");
        return -1;
    }

    while (!quit) {

        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
