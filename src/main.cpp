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
#include "lighting/lighting.hpp"
#include "lighting/pointLight.hpp"
#include "lighting/lightCaster.hpp"
#include "lighting/utils.hpp"

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

    // init lighting shader
    Shader lightingVertexShader = Shader(vertex, std::string(ASSETS_PATH) + "shaders/gouraudObj.vert");
    Shader lightingFragmentShader = Shader(fragment, std::string(ASSETS_PATH) + "shaders/gouraudObj.frag");
    ShaderProgram lightingShader = ShaderProgram(lightingVertexShader, lightingFragmentShader);

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
    //lighting.addPointLight(PointLight(glm::vec3(1.0f, 1.2f, 0.0f), 0.5f, 1.0f));
    lighting.addLightCaster(LightCaster(glm::vec3(-0.2f, -1.0f, -0.3f), 1.0f));
    

    // ============================ RENDERING SECTION =====================================

    glClearColor(0, 0, 0, 1.0f);

    // Activate defined shader program 
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    SDL_SetRelativeMouseMode(SDL_TRUE);
    
    while (!window.isQuit()) {

        float currFrame = (float) SDL_GetTicks64();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        camera.updateCameraSpeed(deltaTime);

        handleInput(window, camera);

        
        
        // Clear depth buffer from previous iteration
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handle models and lighting

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        // render lights TODO: add option to toggle this off
        // lighting.updateView(view);
        // lighting.updateProjection(projection);
        // lighting.drawPointLights(pointLightShader);

        // render model
        lightingShader.use();
        lightingShader.setUniform("viewPos", camera.getCameraPos());
        lightingShader.setUniform("view", view);
        lightingShader.setUniform("projection", projection);
        lightingShader.setUniform("model", objModel.getModelMatrix());
        lightingShader.setUniform("normalMatrix", objModel.getNormalMatrix());
        lighting.setLightingUniforms(lightingShader);
        objModel.draw(lightingShader);

        // OpenGL double buffering buffer swap
        window.swapWindow();
    }

    window.closeWindow();

    return 0;
};
