#include "shader.hpp"
#include <iostream>
#include <SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Surface* winSurface = NULL; 
    bool quit = false;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Error initializing SDL:" << SDL_GetError() << std::endl;
    }

    // Access to OpenGL 4.6
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    // Prevent screen flickering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window = SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Failed to create a window! Error: " << SDL_GetError() << std::endl;
        system("pause");
        return -1;
    }

    winSurface = SDL_GetWindowSurface(window);
    
    if (!winSurface) {
        std::cerr << "Error getting surface! Error: " << SDL_GetError() << std::endl;
        system("pause");
        return -1;
    }

    SDL_GLContext mainContext = SDL_GL_CreateContext(window);

    // Check current version of OpenGL
    const GLubyte* version = glGetString(GL_VERSION);  
    if (version) {
        std::cout << "OpenGL Version: " << version << std::endl;
    } 

    // Enable V-Sync
    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW failed to initialize! Error: " << glewGetErrorString(glewInit()) << std::endl;
        return -1;
    }

    glClearColor(0, 0, 0, 1.0f);

   float vertices[] = {
        0.5f, 0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, // bottom left
       -0.5f, 0.5f, 0.0f   // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Vertex Array Object: Manages vertex attribute configurations and bindings to VBOs and EBOs.
    unsigned int VAO;
    // Vertex Buffer Object: Stores vertex data like positions, normals, and texture coordinates in GPU memory.
    unsigned int VBO;
    // Element Buffer Object: Holds indices for indexed drawing to optimize memory usage and performance.
    unsigned int EBO;


    // Assigns a unique ID
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Copy vertices array into a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Copy indices into buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attributes pointers which tells OpenGL how it should interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Initialize shader
    Shader shaderProgram = Shader("../../asset/vertex.vert", "../../asset/fragment.frag");


    while (!quit) {

        while (SDL_PollEvent(&event) > 0) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        // Draw object
        shaderProgram.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // OpenGL double buffering buffer clear and swap
        // glClear()
        SDL_GL_SwapWindow(window); 
    }

    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
