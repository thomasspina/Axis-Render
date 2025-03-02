#pragma once

#include <SDL2/SDL.h>

class Window {
    private:
        SDL_Window* window;
        SDL_Surface* winSurface;
        bool quit;
        SDL_Event event;
        SDL_GLContext mainContext;

    public:
        Window();

        void swapWindow();
        void closeWindow();

        void setQuit();
        bool isQuit();
        SDL_Event getEvent();
};
