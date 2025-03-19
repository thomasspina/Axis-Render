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

#include "UIHandler.hpp"

int main(int argc, char* argv[]) {

    Window window = Window();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Enable blending for grid shader
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ============================ INITIALIZATION SECTION =====================================

    UIHandler uiHandler;

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
    lighting.addLightCaster(LightCaster(glm::vec3(-0.2f, -1.0f, -0.3f), 1.0f)); // add a default light caster for the scene not to be dark

    // World grid setup
    GLuint worldGridVao;
    glGenVertexArrays(1, &worldGridVao);
    glBindVertexArray(worldGridVao);
    glBindVertexArray(0);

    // ============================ RENDERING SECTION =====================================

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    ShaderProgram currShader = gouraudShader;

    while (!window.isQuit()) {

        // Clear depth buffer from previous iteration
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        uiHandler.changeModel(objModel, camera);

        currShader = shaders[uiHandler.changeShader()];

        float currFrame = (float) SDL_GetTicks64();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        camera.updateCameraSpeed(deltaTime);

        uiHandler.handleInput(window, camera, *objModel);

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
        if (uiHandler.getShowGrid()) {
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
        window.renderImGui(camera, *objModel, lighting, uiHandler.getModelSelectRef(), uiHandler.getShaderSelectRef(), uiHandler.getShowGridRef());

        // OpenGL double buffering buffer swap
        window.swapWindow();
    }

    window.closeWindow();

    return 0;
};
