#include <stb_image.h>

#include <iostream>
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shaderProgram.hpp"
#include "config.h"
#include "constants.hpp"

const std::string assetsPath = ASSETS_PATH;

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

    // TODO: switch window name
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

    // Flip vertical axis
    stbi_set_flip_vertically_on_load(true);

    // ============================ INITIALIZATION SECTION =====================================

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    Shader vertexShader(Vertex, assetsPath + "shaders/vertex.vert");
    Shader fragmentShader(Fragment, assetsPath + "shaders/fragment.frag");

    ShaderProgram shaderProgram(vertexShader, fragmentShader);


    // Load and create a texture
    GLuint texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping/filtering options on the currently bound texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture
    int w, h, nC;
    GLubyte* data1 = stbi_load((assetsPath + "textures/paperTexture.jpg").c_str(), &w, &h, &nC, 0);

    if (data1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture!" << std::endl;
    }
    stbi_image_free(data1);


    // Load and create a texture
    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // set the texture wrapping/filtering options on the currently bound texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture
    GLubyte* data2 = stbi_load((assetsPath + "textures/stripes.jpg").c_str(), &w, &h, &nC, 0);

    if (data2) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture!" << std::endl;
    }
    stbi_image_free(data2);

    // ============================ RENDERING SECTION =====================================

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!quit) {
        float currFrame = (float) SDL_GetTicks64();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        const float cameraSpeed = 0.05f * deltaTime;
        while (SDL_PollEvent(&event) > 0) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        
        shaderProgram.use();
        shaderProgram.setUniform("texture1", 0);
        shaderProgram.setUniform("texture2", 1);

        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}