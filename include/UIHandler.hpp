#pragma once

#include "camera.hpp"
#include "rendering/model.hpp"
#include "utils/constants.hpp"

class Window;

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

    RotationMode modelRotationMode = RotationMode::NATURAL_ROTATION;

public:
    UIHandler() : selectedModel(modelSelect), selectedShader(shaderSelect) {};

    void handleInput(Window& window, Camera& camera, Model& model);

    void changeModel(std::unique_ptr<Model>& model, Camera& camera);

    int changeShader();

    std::unique_ptr<Model> loadNewModel();

    void setModelSelect(int newModelSelect) { modelSelect = newModelSelect; }
    void setShaderSelect(int newShaderSelect) { shaderSelect = newShaderSelect; }
    void setShowGrid(bool newShowGrid) { showGrid = newShowGrid; }
    void setModelRotationMode(RotationMode newMode) { modelRotationMode = newMode; }

    int getModelSelect() const { return modelSelect; }
    int getShaderSelect() const { return shaderSelect; }
    bool getShowGrid() const { return showGrid; }
    RotationMode getModelRotationMode() const { return modelRotationMode; }
};
