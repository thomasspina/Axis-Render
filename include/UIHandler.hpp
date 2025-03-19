#pragma once

#include "camera.hpp"
#include "window.hpp"
#include "rendering/model.hpp"

class UIHandler {
    private:
        int modelSelect = 2;
        int shaderSelect = 1;

        int selectedModel;
        int selectedShader;

        bool relativeMouseMode = false;
        bool isUiCollapsed = false;
        bool showGrid = true;

        // ShaderProgram currShader = gouraudShader;

    public:
        UIHandler() : selectedModel(modelSelect), selectedShader(shaderSelect) {};

        void handleInput(Window& window, Camera& camera, Model& model);

        void changeModel(std::unique_ptr<Model>& model, Camera& camera);

        int changeShader();

        std::unique_ptr<Model> loadNewModel();

        int& getModelSelectRef() { return modelSelect; }
        int& getShaderSelectRef() { return shaderSelect; }
        bool& getShowGridRef() { return showGrid; }

        bool getShowGrid() { return showGrid; }
};
