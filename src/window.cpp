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

    glClearColor(0, 0, 0, 1.0f);
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

// double App::getMemoryUsage() const {
//     #if defined(_WIN32)
//         PROCESS_MEMORY_COUNTERS_EX pmc;
//         GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        
//         return static_cast<float>(pmc.WorkingSetSize) / (1024.0f * 1024.0f); // convert to megabytes
//     #elif defined(__APPLE__)
//         struct task_basic_info t_info;
//         mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

//         // return 0 if process info returns error kernel return value
//         if (KERN_SUCCESS != task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count))
//             return 0.0;

//         // return process memory usage in MB
//         return static_cast<float>(t_info.resident_size) / (1024.0f * 1024.0f); // convert to megabytes
//     #endif
// }

// void App::drawPerformanceUI() {
//     float currentFPS = ImGui::GetIO().Framerate;
        
//     // update FPS history
//     fpsHistory[fpsOffset] = currentFPS;
//     fpsOffset = (fpsOffset + 1) % FPS_HISTORY_SIZE;
    
//     // draw fps info
//     ImGui::Text("Frames Per Second (FPS) : %.1f", currentFPS);
//     ImGui::Text("Application average %.3f ms/frame", 1000.0f / currentFPS);
//     ImGui::PlotLines("FPS", 
//         fpsHistory, 
//         FPS_HISTORY_SIZE, 
//         fpsOffset,
//         "", 
//         0.0f,           
//         SCREEN_FPS + 10.0f,         
//         ImVec2(0, 30)
//     );

//     ImGui::Separator();

//     float currentMemory = getMemoryUsage();

//     // update memory history
//     memoryHistory[memoryOffset] = currentMemory;
//     memoryOffset = (memoryOffset + 1) % MEMORY_HISTORY_SIZE;

//     // draw memory info
//     ImGui::Text("Memory Usage: %.2f MB", currentMemory);
//     ImGui::PlotLines("RAM", 
//         memoryHistory, 
//         MEMORY_HISTORY_SIZE, 
//         memoryOffset,
//         "", 
//         0.0f,           
//         AVG_MEMORY_USAGE + 20.0f,         
//         ImVec2(0, 30)
//     );
// }

// void App::drawConfigUI(BoidScreen& boidScreen) {
//     FlockingBehavior& flocking = FlockingBehavior::getInstance();
//     int numBoids = boidScreen.getNumBoids();
//     if (ImGui::SliderInt("Number of Boids", &numBoids, 1, MAX_NUM_BOIDS)) {
//         boidScreen.setNumBoids(numBoids);
//     }

//     float neighbourhoodRadius = boidScreen.getBoidNeighbourhoodRadius();
//     if (ImGui::SliderFloat("Neighbourhood", &neighbourhoodRadius, BOID_DEFAULT_NEIGHBOURHOOD_RADIUS, BOID_MAXIMUM_NEIGHBOURHOOD_RADIUS)) {
//         boidScreen.setBoidNeighbourhoodRadius(neighbourhoodRadius);
//     }

//     float maxSpeed = boidScreen.getMaxBoidSpeed();
//     if (ImGui::SliderFloat("Max Speed", &maxSpeed, BOID_DEFAULT_MIN_SPEED, BOID_MAXIMUM_MAX_SPEED)) {
//         boidScreen.setMaxBoidSpeed(maxSpeed);
//     }

//     ImGui::SliderFloat("Wander Factor", flocking.getWanderFactorPointer(), 0.f, BOID_WANDER_FORCE_FACTOR_MAX);
//     ImGui::Checkbox("Enable Boid Wandering", flocking.getIsWanderEnabledPointer());

//     ImGui::Separator();
//     ImGui::Checkbox("Show Boid Neighbourhood", &drawBoundary);
//     ImGui::Checkbox("Avoid Screen Boundary", boidScreen.getIsAvoidingScreenEdgesPointer());
//     ImGui::Checkbox("Show Grid", &drawGrid);
//     ImGui::Checkbox("Enable Mouse Avoidance (Hold L-click)", flocking.getIsMouseAvoidanceEnabledPointer());
// }

// void App::drawRulesUI() {
//     FlockingBehavior& flocking = FlockingBehavior::getInstance();
//     ImGui::Text("Flocking Rules");
    
//     ImGui::SliderFloat("Separation Radius", flocking.getSeparationRadiusPointer(), BOID_DEFAULT_RADIUS, FLOCK_MAXIMUM_SEPARATION_RADIUS);
//     ImGui::SliderFloat("Avoid Factor", flocking.getSeparationAvoidFactorPointer(), 0.f, FLOCK_MAXIMUM_AVOID_FACTOR);
//     ImGui::Checkbox("Separation Enabled", flocking.getIsSeparationEnabledPointer());
//     ImGui::Separator();

//     ImGui::SliderFloat("Matching Factor", flocking.getMatchingFactorPointer(), 0.f, FLOCK_MAXIMUM_MATCHING_FACTOR);
//     ImGui::Checkbox("Alignment Enabled", flocking.getIsAlignmentEnabledPointer());
//     ImGui::Separator();

//     ImGui::SliderFloat("Centering Factor", flocking.getCenteringFactorPointer(), 0.f, FLOCK_MAXIMUM_CENTERING_FACTOR);
//     ImGui::Checkbox("Cohesion Enabled", flocking.getIsCohesionEnabledPointer());
//     ImGui::Separator();
// }

void Window::drawUI() {
    ImGui::Begin("Engine Menu");

    ImGui::SetWindowPos(ImVec2(1000, 20), ImGuiCond_Once);
    ImGui::SetWindowSize(ImVec2(400, 700), ImGuiCond_Once);

    ImGui::SetNextItemOpen(true, ImGuiCond_Once); // open menu by default
    if (ImGui::CollapsingHeader("Performance")) {
        // drawPerformanceUI();
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once); // open menu by default
    if (ImGui::CollapsingHeader("Rules")) {
        // drawRulesUI();
    }

    ImGui::End();
}

void Window::renderImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // bool show_demo_window = true;
    // ImGui::ShowDemoWindow(&show_demo_window);
    drawUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

