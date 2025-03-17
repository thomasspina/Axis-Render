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


void handleInput(Window& window, Camera& camera, Model& model) {
    SDL_Event event = window.getEvent();

    while (SDL_PollEvent(&event) > 0) {

        ImGui_ImplSDL2_ProcessEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                window.setQuit();
                break;
                
            case SDL_KEYDOWN: {

                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                        relativeMouseMode = false;
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

int main(int argc, char* argv[]) {

    Window window = Window();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Enable blending for grid shader TODO: does it disturb other shaders?
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ============================ INITIALIZATION SECTION =====================================
    // stbi_set_flip_vertically_on_load(true);

    // gouraud lighting shader
    ShaderProgram gouraudShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/gouraudObj.vert", std::string(ASSETS_PATH) + "shaders/gouraudObj.frag");

    // phong lighting shader
    ShaderProgram phongShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/phongObj.vert", std::string(ASSETS_PATH) + "shaders/phongObj.frag");

    // init PointLight shader
    ShaderProgram pointLightShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/pointLight.vert", std::string(ASSETS_PATH) + "shaders/pointLight.frag");

    // world grid shader
    ShaderProgram worldGridShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/worldGrid.vert", std::string(ASSETS_PATH) + "shaders/worldGrid.frag");


    // Create a model
    std::unique_ptr<Model> objModel = std::make_unique<Model>(std::string(ASSETS_PATH) + "models/Space Shuttle/Space Shuttle.obj");
    objModel->setModelName(3);
    // Model objModel = Model(std::string(ASSETS_PATH) + "models/backpack/backpack.obj");
    // Model objModel = Model(std::string(ASSETS_PATH) + "models/brickCylinder/brickCylinder.obj");
    // Model objModel = Model(std::string(ASSETS_PATH) + "models/cube/cube.obj");

    // Create a camera object
    Camera camera = Camera(objModel->getModelRadius(), objModel->getModelCenter());

    // Create a lighting object
    Lighting lighting = Lighting();
    lighting.addLightCaster(LightCaster(glm::vec3(-0.2f, -1.0f, -0.3f), 1.5f));

    // World grid setup
    GLuint worldGridVao;
    glGenVertexArrays(1, &worldGridVao);
    glBindVertexArray(worldGridVao);
    glBindVertexArray(0);



    // ============================ RENDERING SECTION =====================================

    // Activate defined shader program 
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!window.isQuit()) {

        // Clear depth buffer from previous iteration
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (objModel->getModelName() != modelSelect) {
            std::string modelName = ModelSelection::models[modelSelect];

            objModel = std::make_unique<Model>(std::string(ASSETS_PATH) + "models/" + modelName + "/" + modelName + ".obj");
            objModel->setModelName(modelSelect);
            camera = Camera(objModel->getModelRadius(), objModel->getModelCenter());
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
        gouraudShader.use();
        objModel->updateModelMatrix();
        objModel->updateNormalMatrix(view);
        gouraudShader.setUniform("view", view);
        gouraudShader.setUniform("projection", projection);
        gouraudShader.setUniform("model", objModel->getModelMatrix());
        gouraudShader.setUniform("normalMatrix", objModel->getNormalMatrix());
        lighting.setLightingUniforms(gouraudShader);
        objModel->draw(gouraudShader);

                // render world grid
        //glDepthMask(GL_FALSE);
        worldGridShader.use();
        worldGridShader.setUniform("view", view);
        worldGridShader.setUniform("projection", projection);
        worldGridShader.setUniform("cameraPos", camera.getCameraPos());
        glBindVertexArray(worldGridVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDepthMask(GL_TRUE);

        // Render UI
        window.renderImGui(camera, *objModel, modelSelect);

        // OpenGL double buffering buffer swap
        window.swapWindow();
    }

    window.closeWindow();

    return 0;
};
