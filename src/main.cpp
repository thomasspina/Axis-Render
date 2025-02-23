#include "constants.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"
#include "config.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Surface* winSurface = NULL; 
    bool quit = false;
    SDL_Event event;
    SDL_GLContext gl_context;

    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Error initializing SDL:" << SDL_GetError() << std::endl;
    }

    window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Failed to create a window! Error: " << SDL_GetError() << std::endl;
        system("pause");
        return -1;
    }


    gl_context = SDL_GL_CreateContext(window);
    glewInit();
    glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    
    float vertices[] = {
        1.0f,  1.0f, 0.0f,  // top right
        1.0f, 0.0f, 0.0f,  // bottom right
        0.0f, 0.0f, 0.0f,  // bottom left
        0.0f,  1.0f, 0.0f   // top left 
    };

    float vertices2[] = {
        0.0f,  1.0f, 0.0f,  // top right
        0.0f, 0.0f, 0.0f,  // bottom right
        -1.0f, 0.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 

    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);
    glBindVertexArray(VAO[0]);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int VBO[2];
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Shader vertexShader = Shader(Vertex, std::string(ASSETS_PATH) + "shaders/test_vertex.vert");
    Shader fragmentShader = Shader(Fragement, std::string(ASSETS_PATH) + "shaders/test_fragment.frag");
    ShaderProgram shaderProgram = ShaderProgram(vertexShader, fragmentShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);


    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    
    
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    while (!quit) {

        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }


        // Clear buffer each frame
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Black background
        glClear(GL_COLOR_BUFFER_BIT);
        
        shaderProgram.use();
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        SDL_GL_SwapWindow(window);
    }
    
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
