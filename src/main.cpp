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

#include "mesh.hpp"

// Function to generate a cube mesh
Mesh createCubeMesh(float size, std::vector<Texture> textures = {}) {
    float halfSize = size * 0.5f;
    
    // Define the 8 vertices of the cube
    std::vector<Vertex> vertices = {
        // Front face
        {{-halfSize, -halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}},  // Bottom-left
        {{ halfSize, -halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom-right
        {{ halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}},  // Top-right
        {{-halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},  // Top-left
        
        // Back face
        {{-halfSize, -halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}}, // Bottom-left
        {{-halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}}, // Top-left
        {{ halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}}, // Top-right
        {{ halfSize, -halfSize, -halfSize}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}}, // Bottom-right
        
        // Top face
        {{-halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},  // Bottom-left
        {{-halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // Top-left
        {{ halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},  // Top-right
        {{ halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom-right
        
        // Bottom face
        {{-halfSize, -halfSize, -halfSize}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Top-left
        {{ halfSize, -halfSize, -halfSize}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}, // Top-right
        {{ halfSize, -halfSize,  halfSize}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Bottom-right
        {{-halfSize, -halfSize,  halfSize}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}, // Bottom-left
        
        // Right face
        {{ halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},  // Bottom-left
        {{ halfSize,  halfSize, -halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // Top-left
        {{ halfSize,  halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},  // Top-right
        {{ halfSize, -halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom-right
        
        // Left face
        {{-halfSize, -halfSize, -halfSize}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Bottom-right
        {{-halfSize, -halfSize,  halfSize}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}, // Bottom-left
        {{-halfSize,  halfSize,  halfSize}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Top-left
        {{-halfSize,  halfSize, -halfSize}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}}  // Top-right
    };
    
    // Define the indices for the 12 triangles (6 faces, 2 triangles each)
    std::vector<GLuint> indices = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        
        // Back face
        4, 5, 6,
        6, 7, 4,
        
        // Top face
        8, 9, 10,
        10, 11, 8,
        
        // Bottom face
        12, 13, 14,
        14, 15, 12,
        
        // Right face
        16, 17, 18,
        18, 19, 16,
        
        // Left face
        20, 21, 22,
        22, 23, 20
    };
    
    // Create and return the mesh
    return Mesh(vertices, indices, textures);
}

int main(int argc, char* argv[]) {

    Window window = Window();
    glEnable(GL_DEPTH_TEST);

    // ============================ INITIALIZATION SECTION =====================================

    // Initialize shader
    Shader vertexShader = Shader(vertex, std::string(ASSETS_PATH) + "shaders/vertexLight.vert");
    Shader fragmentShader = Shader(fragment, std::string(ASSETS_PATH) + "shaders/fragmentLight.frag");
    ShaderProgram shaderProgram = ShaderProgram(vertexShader, fragmentShader);

    // Create a cube mesh
    Mesh cubeMesh = createCubeMesh(1.0f);

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

        int viewLoc = glGetUniformLocation(shaderProgram.ID(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(shaderProgram.ID(), "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        // In your rendering loop, before drawing the cube
        glm::mat4 model = glm::mat4(1.0f);
        // Add some rotation to see the 3D nature of the cube
        model = glm::rotate(model, (float)SDL_GetTicks() / 1000.0f, glm::vec3(0.5f, 1.0f, 0.0f));

        // Set the model, view, and projection matrices in the shader
        int modelLoc = glGetUniformLocation(shaderProgram.ID(), "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        // Set object color and light color uniforms
        int objectColorLoc = glGetUniformLocation(shaderProgram.ID(), "objectColor");
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f); // Copper-like color

        int lightColorLoc = glGetUniformLocation(shaderProgram.ID(), "lightColor");
        glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // White light

        // Render the cube
        cubeMesh.draw(vertexShader); // TODO: dummy shader

        // OpenGL double buffering buffer swap
        window.swapWindow();
    }

    window.closeWindow();

    return 0;
}