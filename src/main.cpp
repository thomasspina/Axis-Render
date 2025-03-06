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

glm::mat4 createModelMatrix() {
    // In your rendering loop, before drawing the cube
    // Add some rotation to see the 3D nature of the cube
    glm::mat4 model = glm::rotate(IDENTITY_MATRIX, (float)SDL_GetTicks64() / 1000.0f, DEFAULT_ROTATION_AXIS);

    // return model;

    // glm::mat4 model = IDENTITY_MATRIX;


    return model;
    // return glm::translate(model,  glm::vec3(1.0f, 1.0f, 2.0f));
}

glm::mat4 createViewMatrix(Camera& camera) {
    return camera.getLookAtMatrix();
}

glm::mat4 createProjectionMatrix(Camera& camera) {
    return glm::perspective(glm::radians(camera.getFov()), DEFAULT_ASPECT_RATIO, DEFAULT_NEAR_CLIPPING_PLANE, DEFAULT_FAR_CLIPPING_PLANE);
}

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

    // Create a cube mesh
    Model objModel = Model(std::string(ASSETS_PATH) + "models/spaceShuttle/spaceShuttle.obj");
    // Model objModel = Model(std::string(ASSETS_PATH) + "models/backpack/backpack.obj");
    // Model objModel = Model(std::string(ASSETS_PATH) + "models/brickCylinder/brickCylinder.obj");

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
        float currFrame = (float)SDL_GetTicks64();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        camera.updateCameraSpeed(deltaTime);

        handleInput(window, camera);

        // Clear depth buffer from previous iteration
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: Add to camera option (Orbiting camera)
        // const float radius = 10.0f;
        // float camX = sin((float)SDL_GetTicks() / 1000.0f) * radius;
        // float camZ = cos((float)SDL_GetTicks() / 1000.0f) * radius;

        glm::mat4 model = createModelMatrix();
        glm::mat4 view = createViewMatrix(camera);
        glm::mat4 projection = createProjectionMatrix(camera);

        // Set the model, view, and projection matrices in the shader
        shaderProgram.setUniform("model", model);
        shaderProgram.setUniform("view", view);
        shaderProgram.setUniform("projection", projection);

        // Render the cube
        objModel.draw(shaderProgram);

        // OpenGL double buffering buffer swap
        window.swapWindow();
    }

    window.closeWindow();

    return 0;
};
