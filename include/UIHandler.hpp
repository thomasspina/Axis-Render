#pragma once

#include "camera.hpp"
#include "window.hpp"
#include "rendering/model.hpp"

class UIHandler {
    private:
        // Select default model as Space Shuttle
        int modelSelect = 7;

        // Select default shader as Phong
        int shaderSelect = 0;

        int selectedModel;
        int selectedShader;

        bool relativeMouseMode = false;
        bool isUiCollapsed = false;
        bool showGrid = true;

    public:
        UIHandler() : selectedModel(modelSelect), selectedShader(shaderSelect) {};

        void handleInput(Window& window, Camera& camera, Model& model);

        void changeModel(std::unique_ptr<Model>& model, Camera& camera);

        int changeShader();

        std::unique_ptr<Model> loadNewModel();

        int& getModelSelectRef() { return modelSelect; }
        int& getShaderSelectRef() { return shaderSelect; }
        bool& getShowGridRef() { return showGrid; }

        int getModelSelect() { return modelSelect; }
        int getShaderSelect() { return shaderSelect; }
        bool getShowGrid() { return showGrid; }
};
