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

    // Initialize shader
    Shader vertexShader = Shader(vertex, std::string(ASSETS_PATH) + "shaders/model.vert");
    Shader fragmentShader = Shader(fragment, std::string(ASSETS_PATH) + "shaders/model.frag");
    ShaderProgram shaderProgram = ShaderProgram(vertexShader, fragmentShader);

    // Create a model
    Model objModel = Model(std::string(ASSETS_PATH) + "models/spaceShuttle/spaceShuttle.obj");
    // Model objModel = Model(std::string(ASSETS_PATH) + "models/backpack/backpack.obj");
    // Model objModel = Model(std::string(ASSETS_PATH) + "models/bedroom/bedroom.obj");

    // Create a camera object
    Camera camera = Camera(objModel.getModelRadius(), objModel.getModelCenter());

    // ============================ RENDERING SECTION =====================================

    glClearColor(0, 0, 0, 1.0f);

    // Activate defined shader program 
    shaderProgram.use();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!window.isQuit()) {
        float currFrame = (float) SDL_GetTicks64();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        camera.updateCameraSpeed(deltaTime);

        handleInput(window, camera, objModel);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        // Clear depth buffer from previous iteration
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set the model, view, and projection matrices in the shader
        objModel.updateNormalMatrix(view);
        objModel.updateModelMatrix();
        shaderProgram.setUniform("model", objModel.getModel());
        shaderProgram.setUniform("view", view);
        shaderProgram.setUniform("projection", projection);
        shaderProgram.setUniform("normalMatrix", objModel.getNormalMatrix());

        // Render the model
        objModel.draw(shaderProgram);

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
