#pragma once

#include <SDL2/SDL.h>

class Window {
    private:
        SDL_Window* window;
        SDL_Surface* winSurface;
        bool quit;
        SDL_Event event;
        SDL_GLContext mainContext;
        
        void initializeSDL();
        void configureOpenGL();
        void createWindow();
        void createContext();
        void initializeOpenGL();
        void initializeImGui();

        void drawUI();


    public:
        Window();

        void renderImGui();

        void swapWindow();
        void closeWindow();

        void setQuit();
        bool isQuit();
        SDL_Event getEvent();

        SDL_Window* getWindow() const;
};
