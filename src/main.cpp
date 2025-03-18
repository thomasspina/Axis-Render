#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/shaderProgram.hpp"
#include "config.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include "imgui_impl_opengl3.h"

#include "constants.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "rendering/model.hpp"

#include "lighting/lighting.hpp"
#include "lighting/pointLight.hpp"
#include "lighting/lightCaster.hpp"
#include "lighting/utils.hpp"

bool relativeMouseMode = false;
int modelSelect = 2;
int shaderSelect = 1;
int selectedModel = modelSelect;
int selectedShader = shaderSelect;

float scaleValue = 1.0;
float selectedScaleValue = scaleValue;

bool isUiCollapsed = false;

void handleInput(Window& window, Camera& camera, Model& model) {
    SDL_Event event = window.getEvent();

    while (SDL_PollEvent(&event) > 0) {

        ImGui_ImplSDL2_ProcessEvent(&event);

        switch(event.type) {
            case SDL_QUIT:
                window.setQuit();
                break;

            case SDL_WINDOWEVENT:

                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    int width, height;
                    SDL_GetWindowSize(window.getWindow(), &width, &height);

                    glViewport(0, 0, width, height);
                    // camera.setCameraConfiguration();
                }
            
                break;

            case SDL_KEYDOWN: {

                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                        relativeMouseMode = false;
                        break;
                    case SDLK_RETURN:
                        model.resetModel();
                        break;
                    case SDLK_w:
                        camera.move("w");
                        break;
                    case SDLK_a:
                        camera.move("a");
                        break;
                    case SDLK_s:
                        camera.move("s");
                        break;
                    case SDLK_d:
                        camera.move("d");
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
                if (SDL_BUTTON(SDL_BUTTON_LEFT)) {
                    if (!ImGui::GetIO().WantCaptureMouse) { 
                        SDL_SetRelativeMouseMode(SDL_TRUE);
                        relativeMouseMode = true;
                    }
                }

                break;
            }

            case SDL_MOUSEWHEEL: {
                float yOffset = event.wheel.y;
                
                camera.applyZoom(yOffset);

                break;
            }

            case SDL_MOUSEMOTION: {
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
                break;
            }
        }
    }
}

std::unique_ptr<Model> loadNewModel() {
    std::string modelName = ModelSelection::models[modelSelect];
    return std::make_unique<Model>(std::string(ASSETS_PATH) + "models/" + modelName + "/" + modelName + ".obj");
}

int main(int argc, char* argv[]) {

    Window window = Window();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Enable blending for grid shader
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ============================ INITIALIZATION SECTION =====================================

    // gouraud lighting shader
    ShaderProgram gouraudShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/gouraudObj.vert", std::string(ASSETS_PATH) + "shaders/gouraudObj.frag");

    // phong lighting shaders
    ShaderProgram phongShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/phongObj.vert", std::string(ASSETS_PATH) + "shaders/phongObj.frag");

    ShaderProgram sketchShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/gouraudObj.vert", std::string(ASSETS_PATH) + "shaders/sketch.frag");

    ShaderProgram asciiShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/gouraudObj.vert", std::string(ASSETS_PATH) + "shaders/ascii.frag");

    // Selectable shaders
    ShaderProgram shaders[] = { phongShader, gouraudShader, sketchShader, asciiShader };

    // init PointLight shader
    ShaderProgram pointLightShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/pointLight.vert", std::string(ASSETS_PATH) + "shaders/pointLight.frag");

    // world grid shader
    ShaderProgram worldGridShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/worldGrid.vert", std::string(ASSETS_PATH) + "shaders/worldGrid.frag");


    // Create a model
    std::unique_ptr<Model> objModel = std::make_unique<Model>(std::string(ASSETS_PATH) + "models/Space Shuttle/Space Shuttle.obj");
    objModel->setModelName(2);

    // Create a camera object
    Camera camera = Camera(objModel->getModelRadius(), objModel->getModelCenter());

    // Create a lighting object
    Lighting lighting = Lighting();
    lighting.addLightCaster(LightCaster(glm::vec3(-0.2f, -1.0f, -0.3f), 1.0f));

    // World grid setup
    GLuint worldGridVao;
    glGenVertexArrays(1, &worldGridVao);
    glBindVertexArray(worldGridVao);
    glBindVertexArray(0);

    // ============================ RENDERING SECTION =====================================

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    ShaderProgram currShader = gouraudShader;
    bool showGrid = true;

    while (!window.isQuit()) {

        // Clear depth buffer from previous iteration
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (selectedModel != modelSelect) {
            objModel = loadNewModel();
            camera = Camera(objModel->getModelRadius(), objModel->getModelCenter());
            selectedModel = modelSelect;
        }

        if (selectedShader != shaderSelect) {
            currShader = shaders[shaderSelect];
            selectedShader = shaderSelect;
        }

        float currFrame = (float) SDL_GetTicks64();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        camera.updateCameraSpeed(deltaTime);

        handleInput(window, camera, *objModel);

        // Handle models and lighting
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        // render lights TODO: add option to toggle this off
        lighting.updateView(view);
        lighting.updateProjection(projection);
        lighting.drawPointLights(pointLightShader);

        // render model
        currShader.use();

        objModel->updateModelMatrix();
        objModel->updateNormalMatrix(view);

        currShader.setUniform("view", view);
        currShader.setUniform("projection", projection);
        currShader.setUniform("model", objModel->getModelMatrix());
        currShader.setUniform("normalMatrix", objModel->getNormalMatrix());
        lighting.setLightingUniforms(currShader);
        objModel->draw(currShader);

        // render world grid
        if (showGrid) {
            worldGridShader.use();
            worldGridShader.setUniform("view", view);
            worldGridShader.setUniform("projection", projection);
            worldGridShader.setUniform("cameraPos", camera.getCameraPos());
            worldGridShader.setUniform("modelRadius", objModel->getModelRadius());
            glBindVertexArray(worldGridVao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawArrays(GL_LINES, 6, 2);
        }

        // Render UI
        window.renderImGui(camera, *objModel, lighting, modelSelect, shaderSelect, showGrid);

        // OpenGL double buffering buffer swap
        window.swapWindow();
    }

    window.closeWindow();

    return 0;
};
