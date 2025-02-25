#pragma once

#include <string>
#include <GL/glew.h>
#include <stb_image.h>

class Texture {
private:
    GLuint textureID;
    int width, height, nChannels;

    void loadTexture(const std::string& texturePath);

public:
    Texture(const std::string& texturePath);
    
    int getWidth() const;
    int getHeight() const;
    int getNChannels() const;

    GLuint getTextureID() const;

    ~Texture();
};