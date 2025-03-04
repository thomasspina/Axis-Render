#include "shaderProgram.hpp"
#include "config.h"

#include <stb_image.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "constants.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "model.hpp"
#include "mesh.hpp"

int main(int argc, char* argv[]) {

    Window window = Window();
    glEnable(GL_DEPTH_TEST);

    // ============================ INITIALIZATION SECTION =====================================

    // Initialize shader
    Shader vertexShader = Shader(vertex, std::string(ASSETS_PATH) + "shaders/model.vert");
    Shader fragmentShader = Shader(fragment, std::string(ASSETS_PATH) + "shaders/model.frag");
    ShaderProgram shaderProgram = ShaderProgram(vertexShader, fragmentShader);

    // Create a cube mesh
    Model objModel = Model(std::string(ASSETS_PATH) + "models/spaceShuttle/spaceShuttle.obj");

    // ============================ RENDERING SECTION =====================================

    glClearColor(0, 0, 0, 1.0f);

    // Activate defined shader program 
    shaderProgram.use();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    SDL_SetRelativeMouseMode(SDL_TRUE);

    Camera camera = Camera();

    while (!window.isQuit()) {
        float currFrame = (float)SDL_GetTicks64();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        camera.updateCameraSpeed(deltaTime);
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
                    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
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

        // Clear depth buffer from previous iteration
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float radius = 10.0f;
        float camX = sin((float)SDL_GetTicks() / 1000.0f) * radius;
        float camZ = cos((float)SDL_GetTicks() / 1000.0f) * radius;

        glm::mat4 view = glm::lookAt(camera.getCameraPos(), camera.getCameraPos() + camera.getCameraFront(), camera.getGlobalUp());

        // Projection Matrix
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(camera.getFov()), 800.0f / 600.0f, 0.1f, 100.0f);

        shaderProgram.setUniform("view", view);
        shaderProgram.setUniform("projection", projection);
        
        // In your rendering loop, before drawing the cube
        glm::mat4 model = glm::mat4(1.0f);
        // Add some rotation to see the 3D nature of the cube
        model = glm::rotate(model, (float)SDL_GetTicks() / 1000.0f, glm::vec3(0.5f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(1.0f, 1.0f, 2.0f));

        // Set the model, view, and projection matrices in the shader
        shaderProgram.setUniform("model", model);

        // Render the cube
        objModel.draw(shaderProgram);

        // OpenGL double buffering buffer swap
        window.swapWindow();
    }

    window.closeWindow();

    return 0;
};
