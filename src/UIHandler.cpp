#include "UIHandler.hpp"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include "imgui_impl_opengl3.h"
#include "constants.hpp"
#include "config.h"


void UIHandler::handleInput(Window& window, Camera& camera, Model& model) {
    SDL_Event event;
    ImGuiIO& io = ImGui::GetIO();
    bool escapePressed = false;
    
    while (SDL_PollEvent(&event) > 0) {
        if (!relativeMouseMode) {
            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        switch(event.type) {
            case SDL_QUIT:
                window.setQuit();
                break;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int width, height;
                    SDL_GetWindowSize(window.getWindow(), &width, &height);
                    glViewport(0, 0, width, height);
                }
                break;

            case SDL_KEYDOWN: {
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        escapePressed = true;
                        break;
                    case SDLK_RETURN:
                        model.resetModel();
                        break;
                    case SDLK_w:
                        if (relativeMouseMode) camera.move("w");
                        break;
                    case SDLK_a:
                        if (relativeMouseMode) camera.move("a");
                        break;
                    case SDLK_s:
                        if (relativeMouseMode) camera.move("s");
                        break;
                    case SDLK_d:
                        if (relativeMouseMode) camera.move("d");
                        break;
                    case SDLK_r:
                        camera.reset();
                        break;
                    case SDLK_TAB:
                        isUiCollapsed = !isUiCollapsed;
                        ImGui::SetWindowCollapsed("Engine Menu", isUiCollapsed);
                        break;
                }
                break;
            }

            case SDL_MOUSEBUTTONDOWN: {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // Set relative mouse to true if cursor is on context and not on UI
                    if (!relativeMouseMode && !io.WantCaptureMouse) {
                        SDL_SetRelativeMouseMode(SDL_TRUE);
                        relativeMouseMode = true;
                    }
                }
                break;
            }

            case SDL_MOUSEWHEEL: {
                // Separate UI scrolling and camera zooming
                if (!relativeMouseMode && io.WantCaptureMouse) {
                    break;
                }

                float yOffset = event.wheel.y;
                camera.applyZoom(yOffset);
                break;
            }

            case SDL_MOUSEMOTION: {
                if (relativeMouseMode || 
                    (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) ||
                    (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))) {
                    
                    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
                        float xOffset = event.motion.xrel * DEFAULT_MODEL_ROTATION_SENSITIVITY;
                        float yOffset = event.motion.yrel * DEFAULT_MODEL_ROTATION_SENSITIVITY;
                        model.updateObjectYaw(xOffset);
                        model.updateObjectPitch(yOffset);
                    } else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                        float xOffset = event.motion.xrel;
                        float yOffset = event.motion.yrel;
                        camera.applyRotation(xOffset, yOffset);
                    }
                }
                break;
            }
        }
    }

    if (escapePressed && relativeMouseMode) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        relativeMouseMode = false;
        
        // Set left mouse button as not pressed
        io.MouseDown[0] = false;
    }
}

void UIHandler::changeModel(std::unique_ptr<Model>& model, Camera& camera) {
    if (selectedModel != modelSelect) {
        model = loadNewModel();
        camera = Camera(model->getModelRadius(), model->getModelCenter());
        selectedModel = modelSelect;
    }
}

int UIHandler::changeShader() {
    if (selectedShader != shaderSelect) {
        selectedShader = shaderSelect;
    }

    return selectedShader;
}

std::unique_ptr<Model> UIHandler::loadNewModel() {
    std::string modelName = ModelSelection::models[modelSelect];
    return std::make_unique<Model>(std::string(ASSETS_PATH) + "models/" + modelName + "/" + modelName + ".obj");
}
