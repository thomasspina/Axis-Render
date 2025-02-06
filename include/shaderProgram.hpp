#pragma once

#include "shader.hpp"

#include <unordered_map>
#include <memory>

class ShaderProgram {
private:
    std::unordered_map<ShaderType, std::shared_ptr<Shader>> shaders;
};