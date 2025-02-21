#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.hpp"
#include <iostream>
#include <SDL.h>
#include <SDL_timer.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    // ============================ INITIALIZATION SECTION =====================================

    // Initialize shader
    Shader shaderProgram = Shader("../../asset/vertex.vert", "../../asset/fragment.frag");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

    // Vector in world space that points to the camera's position
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    // Vector that represents what the camera is pointing at, Z axis
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

    // Vector that points to the X axis
    glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(globalUp, cameraDirection));

    // Vector that points to the Y axis
    glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));

    // float vertices[] = {
    //     // Positions       // Colors          // Texture Coords
    //     0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
    //     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
    //    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, // bottom left
    //    -0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f  // top left
    // };

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
    // Texture
    unsigned int texture1;
    unsigned int texture2;


    // Assigns a unique ID
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO and store vertice data into the currently bounded buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind to EBO within the vertex array and store indice data
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    int width, height, nrChannels;
    unsigned char *data;

    // TEXTURE 1

    // Bind to Texture state 2D and store texture info to currently bounded texture object
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("../../asset/textures/download.jpg", &width, &height, &nrChannels, 0);

    if (data) {
        // Generate a texture on currently bounded texture object
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    } else{
        std::cerr << "Failed to generate texture!" << std::endl;
    }

    stbi_image_free(data);

    // TEXTURE 2

    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("../../asset/textures/stripes.jpg", &width, &height, &nrChannels, 0);

    if (data) {
        // Generate a texture on currently bounded texture object
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    } else{
        std::cerr << "Failed to generate texture!" << std::endl;
    }

    stbi_image_free(data);

    glEnable(GL_DEPTH_TEST);

    // ============================ RENDERING SECTION =====================================

    glClearColor(0, 0, 0, 1.0f);

    // Set vertex attributes pointers which tells OpenGL how it should interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Set color attribute pointers
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Set texture attribute pointers
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Activate defined shader program 
    shaderProgram.use();

    // Declare active textures and bind the texture objects to them
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture1"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "texture2"), 1);


    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float lastX = 640;
    float lastY = 360;
    bool firstMouse = true;
    float fov = 45.0f;

    SDL_SetRelativeMouseMode(SDL_TRUE);

   
    float yaw = -90.0f;
    float pitch = 0.0f;

    while (!quit) {
        float currFrame = (float)SDL_GetTicks64();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        std::cout << 1000.0f / deltaTime << "fps\n" << std::endl;
        const float cameraSpeed = 0.05f * deltaTime;
        while (SDL_PollEvent(&event) > 0) {
            switch(event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_MOUSEWHEEL: {
                    float yOffset = event.wheel.y;
                    fov -= yOffset;

                    if (fov < 1.0f) {
                        fov = 1.0f;
                    }

                    if (fov > 45.0f) {
                        fov = 45.0f;
                    }

                    break;
                }

                case SDL_MOUSEMOTION: {
                    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                        float xOffset = event.motion.xrel;
                        float yOffset = event.motion.yrel;

                        const float sensitivity = 0.1f;
                        xOffset *= sensitivity;
                        yOffset *= sensitivity;

                        yaw += xOffset;
                        pitch += yOffset;

                        // Clamp pitch to avoid flipping
                        if (pitch > 89.0f)
                            pitch = 89.0f;
                        if (pitch < -89.0f)
                            pitch = -89.0f;

                        glm::vec3 direction;
                        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                        direction.y = sin(glm::radians(pitch));
                        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                        cameraFront = glm::normalize(direction);
                    }
                    break;
                }
                case SDL_KEYDOWN: {
                    switch(event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                            cameraPos += cameraSpeed * cameraFront;
                            break;
                        case SDL_SCANCODE_A:
                            cameraPos -= glm::normalize(glm::cross(cameraFront, globalUp)) * cameraSpeed;
                            break;
                        case SDL_SCANCODE_S:
                            cameraPos -= cameraSpeed * cameraFront;
                            break;
                        case SDL_SCANCODE_D:
                            cameraPos += glm::normalize(glm::cross(cameraFront, globalUp)) * cameraSpeed;
                            break;
                    }
                }
            }
        }

        // Clear depth buffer from previous iteration
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Model Matrix
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::rotate(model, (float)SDL_GetTicks() / 1000.0f * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f));

        const float radius = 10.0f;
        float camX = sin((float)SDL_GetTicks() / 1000.0f) * radius;
        float camZ = cos((float)SDL_GetTicks() / 1000.0f) * radius;
        // Transformation matrix to convert vectors into view space
        // glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0, camZ), cameraTarget, globalUp);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, globalUp);

        // Projection Matrix
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);


        // int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO); // Activate VAO
        for (unsigned int i=0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * (i+1);
            model = glm::rotate(model, (float)SDL_GetTicks() / 1000.0f * glm::radians(50.0f) * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0); // Unbind VAO

        // OpenGL double buffering buffer swap
        SDL_GL_SwapWindow(window); 
    }

    // TESTING CODE:

     // Flucuating colors
    // const Uint64 time = SDL_GetTicks64() / 1000.0f;
    // float colorValue = (sin(time) / 2.0f) + 0.5f;
    // int vertexColorLocation = glGetUniformLocation(shaderProgram.ID, "ourColor");
    // glUniform4f(vertexColorLocation, 0.0f, colorValue, 0.0f, 1.0f);


    // Object uniform transformation
    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::translate(trans, glm::vec3(0.5f, -0.5, 0.0f));
    // trans = glm::rotate(trans, (float)SDL_GetTicks() / 1000.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    // unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
