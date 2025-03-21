#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

#include "config.h"
#include "camera.hpp"
#include "window.hpp"
#include "UIHandler.hpp"
#include "rendering/model.hpp"
#include "shader/shaderProgram.hpp"
#include "lighting/lighting.hpp"
#include "utils/constants.hpp"


int main(int argc, char* argv[]) {

    Window window = Window();
    
    // ============================ INITIALIZATION SECTION =====================================

    UIHandler uiHandler;

    // gouraud lighting shader
    ShaderProgram gouraudShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/gouraudObj.vert", std::string(ASSETS_PATH) + "shaders/gouraudObj.frag");

    // phong lighting shaders
    ShaderProgram phongShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/phongObj.vert", std::string(ASSETS_PATH) + "shaders/phongObj.frag");
    ShaderProgram grayscaleShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/gouraudObj.vert", std::string(ASSETS_PATH) + "shaders/grayscale.frag");
    ShaderProgram sketchShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/gouraudObj.vert", std::string(ASSETS_PATH) + "shaders/sketch.frag");
    ShaderProgram asciiShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/gouraudObj.vert", std::string(ASSETS_PATH) + "shaders/ascii.frag");

    // Selectable shaders
    ShaderProgram shaders[] = { phongShader, gouraudShader, grayscaleShader, sketchShader, asciiShader };

    // init PointLight shader
    ShaderProgram pointLightShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/pointLight.vert", std::string(ASSETS_PATH) + "shaders/pointLight.frag");

    // world grid shader
    ShaderProgram worldGridShader = ShaderProgram(std::string(ASSETS_PATH) + "shaders/worldGrid.vert", std::string(ASSETS_PATH) + "shaders/worldGrid.frag");

    // Create a model
    std::unique_ptr<Model> objModel = std::make_unique<Model>(std::string(ASSETS_PATH) + "models/" + ModelSelection::models[uiHandler.getModelSelect()] + "/" + ModelSelection::models[uiHandler.getModelSelect()] + ".obj");

    // Create a camera object
    Camera camera = Camera(objModel->getModelRadius(), objModel->getModelCenter());

    // Create a lighting object
    Lighting lighting = Lighting(&camera, objModel.get());

    // World grid setup
    GLuint worldGridVao;
    glGenVertexArrays(1, &worldGridVao);
    glBindVertexArray(worldGridVao);
    glBindVertexArray(0);

    // ============================ RENDERING SECTION =====================================

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    ShaderProgram currShader = shaders[uiHandler.getShaderSelect()];

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

        lighting.setView(view);
        lighting.setProjection(projection);
        lighting.drawPointLights(pointLightShader);

        // render model
        currShader.use();
        if (uiHandler.getModelRotationMode() == RotationMode::NATURAL_ROTATION) {
            const float rotationSpeed = 30.0f;
    
            float rotationAngle = rotationSpeed * (deltaTime / 1000.0f); 
            
            objModel->rotate(rotationAngle, DEFAULT_ROTATION_AXIS);
        } 
        objModel->updateNormalMatrix(view);
        currShader.setUniform("view", view);
        currShader.setUniform("projection", projection);
        currShader.setUniform("model", objModel->getModelMatrix());
        currShader.setUniform("normalMatrix", objModel->getNormalMatrix());
        lighting.setUniformsForShaderProgram(currShader);
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
        window.renderImGui(camera, *objModel, lighting, uiHandler);

        // OpenGL double buffering buffer swap
        window.swapWindow();
    }

    window.closeWindow();

    return 0;
};
