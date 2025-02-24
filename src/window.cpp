#include "window.hpp"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

Window::Window() {
    window = NULL;
    winSurface = NULL; 
    quit = false;

     if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Error initializing SDL:" << SDL_GetError() << std::endl;
    }

    // Access to OpenGL 4.6
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // Prevent screen flickering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Failed to create a window! Error: " << SDL_GetError() << std::endl;
        system("pause");
    }

    winSurface = SDL_GetWindowSurface(window);
    
    if (!winSurface) {
        std::cerr << "Error getting surface! Error: " << SDL_GetError() << std::endl;
        system("pause");
    }

    mainContext = SDL_GL_CreateContext(window);

    // Check current version of OpenGL
    const GLubyte* version = glGetString(GL_VERSION);  
    if (version) {
        std::cout << "OpenGL Version: " << version << std::endl;
    } 

    // Enable V-Sync
    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW failed to initialize! Error: " << glewGetErrorString(glewInit()) << std::endl;
    }
}

void Window::swapWindow() {
    SDL_GL_SwapWindow(window);
}

void Window::closeWindow() {
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::setQuit() {
    quit = true;
}

bool Window::isQuit() {
    return quit;
}

SDL_Event Window::getEvent() {
    return event;
}


