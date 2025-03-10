#pragma once

#include <SDL2/SDL.h>
#include "object.hpp"

class Window {
    private:  
        // FPS UI graph
        static const int FPS_HISTORY_SIZE = 600;
        float fpsHistory[FPS_HISTORY_SIZE] = {0};
        int fpsOffset = 0;

        // Memory UI graph
        static const int MEMORY_HISTORY_SIZE = 600;
        float memoryHistory[MEMORY_HISTORY_SIZE] = {0};
        int memoryOffset = 0;

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

        void drawUI(Object& obj);

        double getMemoryUsage() const;
        void drawPerformanceUI();
        void drawCameraUI();
        void drawModelUI(Object& obj);

    public:
        Window();

        void renderImGui(Object& obj);

        void swapWindow();
        void closeWindow();

        void setQuit();
        bool isQuit();
        SDL_Event getEvent();

        SDL_Window* getWindow() const;
};
