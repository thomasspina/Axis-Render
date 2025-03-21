#pragma once

#include <SDL2/SDL.h>

#include "lighting/lighting.hpp"
#include "object.hpp"
#include "camera.hpp"

class UIHandler;

/**
 * @class Window
 * @brief Manages the application window, OpenGL context, and user interface
 * 
 * The Window class handles window creation, OpenGL initialization, ImGui setup,
 * and UI rendering. It provides functionality for performance monitoring,
 * camera controls, model manipulation, and lighting adjustments.
 */
class Window {
private:  
    // FPS UI graph
    static const int FPS_HISTORY_SIZE = 600;      ///< Size of the array storing framerate history
    float fpsHistory[FPS_HISTORY_SIZE] = {0};     ///< Array storing historical FPS values for plotting
    int fpsOffset = 0;                           ///< Current position in the FPS history array

    // Memory UI graph
    static const int MEMORY_HISTORY_SIZE = 600;   ///< Size of the array storing memory usage history
    float memoryHistory[MEMORY_HISTORY_SIZE] = {0}; ///< Array storing historical memory usage values
    int memoryOffset = 0;                        ///< Current position in the memory history array

    SDL_Window* window;                           ///< Pointer to the SDL window
    SDL_Surface* winSurface;                      ///< Pointer to the window surface
    bool quit;                                    ///< Flag indicating if the application should exit
    SDL_Event event;                              ///< SDL event structure for handling inputs
    SDL_GLContext mainContext;                    ///< SDL OpenGL context
    
    /**
     * @brief Initializes SDL and creates a window
     * 
     * Sets up SDL, configures OpenGL attributes, creates the window,
     * and establishes the OpenGL context.
     */
    void initializeSDL();
    
    /**
     * @brief Sets OpenGL attributes for the context
     * 
     * Configures OpenGL version, double buffering, and depth buffer.
     */
    void configureOpenGL();
    
    /**
     * @brief Creates the SDL window with specified parameters
     * 
     * Creates a resizable window with OpenGL support and centered position.
     */
    void createWindow();
    
    /**
     * @brief Creates the OpenGL context for rendering
     * 
     * Establishes the main rendering context for the window.
     */
    void createContext();
    
    /**
     * @brief Initializes OpenGL features and settings
     * 
     * Enables V-Sync, initializes GLEW, sets clear color, and enables
     * depth testing and alpha blending.
     */
    void initializeOpenGL();
    
    /**
     * @brief Sets up the ImGui framework for UI rendering
     * 
     * Initializes ImGui context and configures it for SDL and OpenGL.
     */
    void initializeImGui();

    /**
     * @brief Main UI rendering method that manages all UI panels
     * 
     * Draws performance, camera, model, and lighting UI sections.
     * 
     * @param camera Reference to the camera object for control settings
     * @param obj Reference to the model object for manipulation
     * @param lighting Reference to the lighting system
     * @param uiHandler Reference to the UI state handler
     */
    void drawUI(Camera& camera, Model& obj, Lighting& lighting, UIHandler& uiHandler);

    /**
     * @brief Gets current application memory usage
     * 
     * Platform-specific implementation to retrieve memory usage in megabytes.
     * Supports Windows and Linux platforms.
     * 
     * @return Current memory usage in MB
     */
    double getMemoryUsage() const;
    
    /**
     * @brief Renders performance metrics (FPS and memory usage)
     * 
     * Displays current FPS, frame time, and memory usage with historical graphs.
     */
    void drawPerformanceUI();
    
    /**
     * @brief Renders camera control UI elements
     * 
     * Creates UI controls for camera settings including rotation, movement,
     * and reset functionality.
     * 
     * @param camera Reference to the camera to control
     */
    void drawCameraUI(Camera& camera);
    
    /**
     * @brief Renders model manipulation UI elements
     * 
     * Creates UI controls for model rotation, scaling, and selection.
     * Also handles shader selection and grid display options.
     * 
     * @param obj Reference to the model to manipulate
     * @param uiHandler Reference to the UI state handler
     */
    void drawModelUI(Model& obj, UIHandler& uiHandler);
    
    /**
     * @brief Renders lighting control UI elements
     * 
     * Creates UI controls for directional and point light properties
     * including direction, intensity, color, and position.
     * 
     * @param lighting Reference to the lighting system
     */
    void drawLightingUI(Lighting& lighting);

public:
    /**
     * @brief Constructs a new Window with SDL, OpenGL, and ImGui initialized
     * 
     * Creates a window with default settings and initializes all required
     * rendering subsystems.
     */
    Window();

    /**
     * @brief Renders the ImGui interface elements
     * 
     * Prepares ImGui for a new frame, draws all UI elements, and renders
     * the resulting interface.
     * 
     * @param camera Reference to the camera for UI controls
     * @param obj Reference to the model for UI controls
     * @param lighting Reference to the lighting system for UI controls
     * @param uiHandler Reference to the UI state handler
     */
    void renderImGui(Camera& camera, Model& obj, Lighting& lighting, UIHandler& uiHandler);

    /**
     * @brief Sets the window to fullscreen mode
     * 
     * Switches to fullscreen desktop mode and recreates the GL context.
     */
    void setWindowFullscreen();
    
    /**
     * @brief Restores the window to windowed mode
     * 
     * Exits fullscreen mode and returns to normal windowed display.
     */
    void setWindowRestore();

    /**
     * @brief Swaps the front and back buffers
     * 
     * Updates the window's displayed content after rendering.
     */
    void swapWindow();
    
    /**
     * @brief Cleans up resources and closes the window
     * 
     * Shuts down ImGui, destroys the GL context and SDL window.
     */
    void closeWindow();

    /**
     * @brief Sets the quit flag to true
     * 
     * Signals that the application should terminate.
     */
    void setQuit();
    
    /**
     * @brief Checks if the application should quit
     * 
     * @return True if the application should terminate, false otherwise
     */
    bool isQuit();
    
    /**
     * @brief Gets the current SDL event
     * 
     * @return The most recent SDL_Event
     */
    SDL_Event getEvent();

    /**
     * @brief Gets the SDL window pointer
     * 
     * @return Pointer to the SDL_Window
     */
    SDL_Window* getWindow() const;
};
