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

#include "constants.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "model.hpp"
#include "mesh.hpp"


void handleInput(Window& window, Camera& camera) {
    SDL_Event event = window.getEvent();

    while (SDL_PollEvent(&event) > 0) {

        switch(event.type) {
            case SDL_QUIT:
                window.setQuit();
                // quit = true;
                break;

            case SDL_MOUSEWHEEL: {
                float yOffset = event.wheel.y;
                
                camera.applyZoom(yOffset);

                break;
            }

            case SDL_MOUSEMOTION: {
                if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE)) {
                    float xOffset = event.motion.xrel * DEFAULT_MODEL_ROTATION_SENSITIVITY;
                    float yOffset = event.motion.yrel * DEFAULT_MODEL_ROTATION_SENSITIVITY;

                } else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                    float xOffset = event.motion.xrel;
                    float yOffset = event.motion.yrel;
                    
                    camera.applyRotation(xOffset, yOffset);
                }
                break;
            }
            case SDL_KEYDOWN: {

                switch(event.key.keysym.scancode) {
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

    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (!window.isQuit()) {
        float currFrame = (float) SDL_GetTicks64();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        camera.updateCameraSpeed(deltaTime);

        handleInput(window, camera);

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        // Clear depth buffer from previous iteration
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set the model, view, and projection matrices in the shader
        objModel.updateNormalMatrix(view);
        shaderProgram.setUniform("model", objModel.getModel());
        shaderProgram.setUniform("view", view);
        shaderProgram.setUniform("projection", projection);
        shaderProgram.setUniform("normalMatrix", objModel.getNormalMatrix());

        // Render the model
        objModel.draw(shaderProgram);

        // OpenGL double buffering buffer swap
        window.swapWindow();
    }

    window.closeWindow();

    return 0;
};
