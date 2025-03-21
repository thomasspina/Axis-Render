#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <memory>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include "imgui_impl_opengl3.h"
#include <string>

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#elif __linux__
    #include <unistd.h>
    #include <stdio.h>
#endif

#include "window.hpp"
#include "UIHandler.hpp"
#include "utils/constants.hpp"

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
    window = SDL_CreateWindow("Axis Render", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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

    // Enables depth comparison, only rendering closes fragments
    glEnable(GL_DEPTH_TEST);

    // Enable blending for grid shader
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

double Window::getMemoryUsage() const {
    #if defined(_WIN32)
       PROCESS_MEMORY_COUNTERS_EX pmc;
       GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
       return static_cast<float>(pmc.WorkingSetSize) / (1024.0f * 1024.0f); // convert to megabytes
    #elif defined(__linux__)
       FILE* file = fopen("/proc/self/statm", "r");
       if (file == NULL)
           return 0.0;
       
       long rss = 0;
       if (fscanf(file, "%*s %ld", &rss) != 1) {
           fclose(file);
           return 0.0;
       }
       fclose(file);
       
       // rss is in pages, convert to bytes and then to MB
       long page_size = sysconf(_SC_PAGESIZE);
       return static_cast<float>(rss * page_size) / (1024.0f * 1024.0f);
    #else
       return 0.0; // Unsupported platform
    #endif
   }

void Window::drawPerformanceUI() {
    float currentFPS = ImGui::GetIO().Framerate;
        
    // update FPS history
    fpsHistory[fpsOffset] = currentFPS;
    fpsOffset = (fpsOffset + 1) % FPS_HISTORY_SIZE;
    
    // draw fps info
    ImGui::Text("Frames Per Second (FPS) : %.1f", currentFPS);
    ImGui::Text("Application average %.3f ms/frame", 1000.0f / currentFPS);
    ImGui::PlotLines("FPS", 
        fpsHistory, 
        FPS_HISTORY_SIZE, 
        fpsOffset,
        "", 
        0.0f,           
        SCREEN_FPS + 10.0f,         
        ImVec2(0, 30)
    );

    ImGui::Separator();

    float currentMemory = getMemoryUsage();

    // update memory history
    memoryHistory[memoryOffset] = currentMemory;
    memoryOffset = (memoryOffset + 1) % MEMORY_HISTORY_SIZE;

    // draw memory info
    ImGui::Text("Memory Usage: %.2f MB", currentMemory);
    ImGui::PlotLines("RAM", 
        memoryHistory, 
        MEMORY_HISTORY_SIZE, 
        memoryOffset,
        "", 
        0.0f,           
        AVG_MEMORY_USAGE + 20.0f,         
        ImVec2(0, 30)
    );
}

void Window::drawCameraUI(Camera& camera) {
    ImGui::TextDisabled("[Scroll] to zoom ");
    ImGui::TextDisabled("[ESC] to release mouse cursor");
    ImGui::TextDisabled("[TAB] to close/open UI");

    bool status = true;
    ImGui::Checkbox("Camera Rotation, [Hold Mouse 2]", camera.getIsCameraRotationEnabled());
    ImGui::Separator();

    // ImGui::SliderFloat("Matching Factor", flocking.getMatchingFactorPointer(), 0.f, FLOCK_MAXIMUM_MATCHING_FACTOR);
    ImGui::Checkbox("Free Camera, [WASD] to move", camera.getIsFreeCameraEnabled());
    ImGui::Separator();

    if (ImGui::Button("Reset Camera [R]")) {
        camera.reset();
    }

    ImGui::Separator();
}

void Window::drawModelUI(Model& obj, UIHandler& uiHandler) {

    RotationMode rotationMode = uiHandler.getModelRotationMode();
    if (ImGui::RadioButton("Input Rotation [Hold Mouse 3]", rotationMode == RotationMode::INPUT_ROTATION)) {
        uiHandler.setModelRotationMode(RotationMode::INPUT_ROTATION);
        obj.resetModel();
    }

    if (ImGui::RadioButton("Natural Rotation", rotationMode == RotationMode::NATURAL_ROTATION)) {
        uiHandler.setModelRotationMode(RotationMode::NATURAL_ROTATION);
    }

    float scale =  obj.getModelScale();
    if (ImGui::SliderFloat("Model Scale", &scale, 0.1, 2.0)) {
        obj.scale(scale);
    }

    if (ImGui::Button("Reset Model [Enter]")) {
        obj.resetModel();
    }

    ImGui::Separator();

    int modelSelect = uiHandler.getModelSelect();
    if (ImGui::Combo("Select Model", &modelSelect, ModelSelection::models, IM_ARRAYSIZE(ModelSelection::models))) {
        uiHandler.setModelSelect(modelSelect);
    }

    ImGui::Separator(); 

    int shaderSelect = uiHandler.getShaderSelect();
    if (ImGui::Combo("Select Shader", &shaderSelect, ShaderSelection::shaders, IM_ARRAYSIZE(ShaderSelection::shaders))) {
        uiHandler.setShaderSelect(shaderSelect);
    }
}

void Window::drawLightingUI(Lighting& lighting) {
    if (ImGui::CollapsingHeader("Light Caster")) {
        LightCaster* lightCaster = lighting.getLightCasterPointer();

        float azimuth = lightCaster->getAzimuth();
        float elevation = lightCaster->getElevation();
    
        if (ImGui::SliderFloat("Azimuth", &azimuth, 0.0f, 360.0f, "%.1f°")) {
            lightCaster->setDirection(azimuth, elevation);
        }
    
        if (ImGui::SliderFloat("Elevation", &elevation, -90.0f, 90.0f, "%.1f°")) {
            lightCaster->setDirection(azimuth, elevation);
        }
    
        float intensity = lightCaster->getIntensity();
        if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 2.0f)) {
            lightCaster->setIntensity(intensity);
        }
    
        glm::vec3 colour = lightCaster->getColour();
        if (ImGui::ColorEdit3("Colour", &colour[0])) {
            lightCaster->setColour(colour);
        }
    
        ImGui::Separator();
    }

    
    if (ImGui::CollapsingHeader("Point Lights")) {
        int nPointLights = lighting.getNPointLights();
        int prev = nPointLights;
        if (ImGui::InputInt("num point lights", &nPointLights, 1.0f)) {
            if (nPointLights < 0) nPointLights = 0;
            if (nPointLights > 4) nPointLights = 4;

            if (nPointLights < prev) {
                for (int i = 0; i < prev - nPointLights; i++) { lighting.removePointLight(); }
            } else {
                for (int i = 0; i < nPointLights - prev; i++) { lighting.addPointLight(); }
            }
        }

        bool showPointLights = lighting.getShowPointLights();
        if (ImGui::Checkbox("Draw Point Lights", &showPointLights)){
            lighting.setShowPointLights(showPointLights);
        }
        ImGui::Separator();

        std::vector<PointLight>& pointLights = *lighting.getPointLightsPointer();
        for (int i = 1; i <= lighting.getNPointLights(); i++) {
            ImGui::Text("Point Light %d", i);
            PointLight& pointLight = pointLights[i - 1];
            
            // Intensity control
            char intensityLabel[50];
            sprintf(intensityLabel, "Intensity %d", i);
            float intensity = pointLight.getIntensity();
            if (ImGui::SliderFloat(intensityLabel, &intensity, 0.0f, 10.0f)) {
                pointLight.setIntensity(intensity);
            }

            // Position control
            char positionLabel[50];
            sprintf(positionLabel, "Position %d", i);
            glm::vec3 position = pointLight.getPosition();
            if (ImGui::DragFloat3(positionLabel, &position[0], 0.1f)) {
                pointLight.setPosition(position);
            }

            // Colour control
            char colourLabel[50];
            sprintf(colourLabel, "Colour %d", i);
            glm::vec3 colour = pointLight.getColour();
            if (ImGui::ColorEdit3(colourLabel, &colour[0])) {
                pointLight.setColour(colour);
            }

            ImGui::Separator();
        }
    }
}

void Window::drawUI(Camera& camera, Model& obj, Lighting& lighting, UIHandler& uiHandler) {
    ImGui::Begin("Engine Menu");

    ImGui::SetWindowPos(ImVec2(875, 20), ImGuiCond_Once);
    ImGui::SetWindowSize(ImVec2(400, 700), ImGuiCond_Once);

    ImGui::SetNextItemOpen(true, ImGuiCond_Once); 
    if (ImGui::CollapsingHeader("Performance")) {
        drawPerformanceUI();
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once); 
    if (ImGui::CollapsingHeader("Camera")) {
        drawCameraUI(camera);
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once); 
    if (ImGui::CollapsingHeader("Model")) {
        drawModelUI(obj, uiHandler);
        
        bool showGrid = uiHandler.getShowGrid();
        if (ImGui::Checkbox("Show Grid", &showGrid)) {
            uiHandler.setShowGrid(showGrid);
        }

    }

    drawLightingUI(lighting);

    ImGui::End();
}

void Window::renderImGui(Camera& camera, Model& obj, Lighting& lighting, UIHandler& uiHandler) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    drawUI(camera, obj, lighting, uiHandler);

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

void Window::setWindowFullscreen() {
    SDL_GL_DeleteContext(mainContext);  // Destroy OpenGL context
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    mainContext = SDL_GL_CreateContext(window);  // Recreate context
    SDL_GL_MakeCurrent(window, mainContext);  // Bind context again
}

void Window::setWindowRestore() {
    SDL_SetWindowFullscreen(window, 0);
}



