#include <stb_image.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderProgram.hpp"
#include "config.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include "imgui_impl_opengl3.h"

#include "constants.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "model.hpp"
#include "mesh.hpp"

bool relativeMouseMode = false;

#include "lighting/lighting.hpp"
#include "lighting/pointLight.hpp"
#include "lighting/lightCaster.hpp"
#include "lighting/utils.hpp"

void toggleRelativeMouseMode() {
    relativeMouseMode = !relativeMouseMode;
    SDL_SetRelativeMouseMode(relativeMouseMode ? SDL_TRUE : SDL_FALSE);
}


void handleInput(Window& window, Camera& camera, Model& model) {
    SDL_Event event = window.getEvent();

    while (SDL_PollEvent(&event) > 0) {

        switch(event.type) {
            case SDL_QUIT:
                window.setQuit();
                // quit = true;
                break;
                
            case SDL_KEYDOWN: {

                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        std::cout << "ESC" << std::endl;
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                        relativeMouseMode = false;
                        break;
                    case SDL_SCANCODE_W:
                        camera.moveForward();
                        break;
                    case SDL_SCANCODE_A:
                        camera.moveLeft();
                        break;
                    case SDL_SCANCODE_S:
                        camera.moveBackward();
                        break;
                    case SDL_SCANCODE_D:
                        camera.moveRight();
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
    glEnable(GL_DEPTH_TEST);

    // ============================ INITIALIZATION SECTION =====================================
    // stbi_set_flip_vertically_on_load(true);

    // gouraud lighting shader
    Shader gouraudVertexShader = Shader(vertex, std::string(ASSETS_PATH) + "shaders/gouraudObj.vert");
    Shader gouraudFragmentShader = Shader(fragment, std::string(ASSETS_PATH) + "shaders/gouraudObj.frag");
    ShaderProgram gouraudShader = ShaderProgram(gouraudVertexShader, gouraudFragmentShader);

    // phong lighting shader
    Shader phongVertexShader = Shader(vertex, std::string(ASSETS_PATH) + "shaders/phongObj.vert");
    Shader phongFragmentShader = Shader(fragment, std::string(ASSETS_PATH) + "shaders/phongObj.frag");
    ShaderProgram phongShader = ShaderProgram(phongVertexShader, phongFragmentShader);

    // init PointLight shader
    Shader pointLightVertexShader = Shader(vertex, std::string(ASSETS_PATH) + "shaders/pointLight.vert");
    Shader pointLightFragmentShader = Shader(fragment, std::string(ASSETS_PATH) + "shaders/pointLight.frag");
    ShaderProgram pointLightShader = ShaderProgram(pointLightVertexShader, pointLightFragmentShader);

    // Create a model
    Model objModel = Model(std::string(ASSETS_PATH) + "models/spaceShuttle/spaceShuttle.obj");
    // Model objModel = Model(std::string(ASSETS_PATH) + "models/backpack/backpack.obj");
    // Model objModel = Model(std::string(ASSETS_PATH) + "models/brickCylinder/brickCylinder.obj");
    // Model objModel = Model(std::string(ASSETS_PATH) + "models/cube/cube.obj");

    // Create a camera object
    Camera camera = Camera(objModel.getModelRadius(), objModel.getModelCenter());

    // Create a lighting object
    Lighting lighting = Lighting();
    lighting.addPointLight(PointLight(glm::vec3(2.0f, 2.2f, 2.0f), 0.5f, 1.0f));
    lighting.addLightCaster(LightCaster(glm::vec3(-0.2f, -1.0f, -0.3f), 1.0f));
    

    // ============================ RENDERING SECTION =====================================

    glClearColor(0, 0, 0, 1.0f);

    // Activate defined shader program 
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!window.isQuit()) {

        float currFrame = (float) SDL_GetTicks64();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        camera.updateCameraSpeed(deltaTime);

        handleInput(window, camera, objModel);

        
        
        // Clear depth buffer from previous iteration
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handle models and lighting

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        // render lights TODO: add option to toggle this off
        lighting.updateView(view);
        lighting.updateProjection(projection);
        lighting.drawPointLights(pointLightShader);

        // render model
        gouraudShader.use();
        gouraudShader.setUniform("view", view);
        gouraudShader.setUniform("projection", projection);
        gouraudShader.setUniform("model", objModel.getModelMatrix());
        gouraudShader.setUniform("normalMatrix", objModel.getNormalMatrix());
        lighting.setLightingUniforms(gouraudShader);
        objModel.draw(gouraudShader);
        objModel.updateModelMatrix();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // OpenGL double buffering buffer swap
        window.swapWindow();
    }

    window.closeWindow();

    return 0;
};
