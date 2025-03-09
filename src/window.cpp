#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

#include "window.hpp"
#include "constants.hpp"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include "imgui_impl_opengl3.h"

Window::Window() {
    quit = false;

    initializeSDL();
    initializeOpenGL();
    initializeImGui();
}

void Window::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Error initializing SDL:" << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    configureOpenGL();
    createWindow();
    createContext();
}

void Window::configureOpenGL() {
    // Access to OpenGL 4.6
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // Prevent screen flickering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void Window::createWindow() {
    window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Failed to creating a window! Error: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Window::createContext() {
    mainContext = SDL_GL_CreateContext(window);

    if (!mainContext) {
        std::cerr << "Error creating context! Error: " << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Window::initializeOpenGL() {
    // Enable V-Sync: Synchronize frame rate of application with the refresh rate of monitor
    SDL_GL_SetSwapInterval(1);

    // Check current version of OpenGL
    const GLubyte* version = glGetString(GL_VERSION);  
    if (version) {
        std::cout << "OpenGL Version: " << version << std::endl;
    } 

    // Init GLEW: Provide access to modern OpenGL features
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "GLEW failed to initialize! Error: " << glewGetErrorString(glewStatus) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Window::initializeImGui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Dark mode
    ImGui::StyleColorsDark();

    // Light mode
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, mainContext);
    ImGui_ImplOpenGL3_Init(OPEN_GL_VERSION);
}

void Window::swapWindow() {
    SDL_GL_SwapWindow(window);
}

void Window::closeWindow() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

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

SDL_Window* Window::getWindow() const {
    return window;
}

