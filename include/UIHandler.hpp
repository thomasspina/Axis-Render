#pragma once

#include "camera.hpp"
#include "rendering/model.hpp"
#include "utils/constants.hpp"

class Window;

/**
 * @brief Handles UI interactions such as model selection, shader changes, and camera controls.
 */
class UIHandler {
private:
    int modelSelect = 7; ///< Select default model as Space Shuttle.
    int shaderSelect = 0; ///< Select default shader as Phong.

    int selectedModel; ///< Selected model index.
    int selectedShader; ///< Selected shader index.

    bool relativeMouseMode = false; ///< Flag to indicate if the mouse is in relative mode.
    bool isUiCollapsed = false; ///< Flag to track if UI is collapsed.
    bool showGrid = true; ///< Flag to determine whether grid should be shown.

    RotationMode modelRotationMode = RotationMode::NATURAL_ROTATION; ///< Model rotation mode (natural or input-based).

public:
    /**
     * @brief Constructs a UIHandler with default selections.
     */
    UIHandler() : selectedModel(modelSelect), selectedShader(shaderSelect) {};

    /**
     * @brief Handles input events like keyboard and mouse input for camera and model controls.
     * 
     * @param window The window object to handle events.
     * @param camera The camera object to move or adjust.
     * @param model The model object to manipulate.
     */
    void handleInput(Window& window, Camera& camera, Model& model);

    /**
     * @brief Changes the current model based on user selection.
     * 
     * @param model A unique pointer to the model to be changed.
     * @param camera The camera object to update based on the new model.
     */
    void changeModel(std::unique_ptr<Model>& model, Camera& camera);

    /**
     * @brief Changes the shader based on the user's selection.
     * 
     * @return The current selected shader index.
     */
    int changeShader();

    /**
     * @brief Loads a new model based on the selected model index.
     * 
     * @return A unique pointer to the newly loaded model.
     */
    std::unique_ptr<Model> loadNewModel();

    /**
     * @brief Sets the model select index.
     * 
     * @param newModelSelect The new model select index.
     */
    void setModelSelect(int newModelSelect) { modelSelect = newModelSelect; }

    /**
     * @brief Sets the shader select index.
     * 
     * @param newShaderSelect The new shader select index.
     */
    void setShaderSelect(int newShaderSelect) { shaderSelect = newShaderSelect; }

    /**
     * @brief Sets the flag to show or hide the grid.
     * 
     * @param newShowGrid The flag to show grid (true to show, false to hide).
     */
    void setShowGrid(bool newShowGrid) { showGrid = newShowGrid; }

    /**
     * @brief Sets the model rotation mode.
     * 
     * @param newMode The new rotation mode for the model.
     */
    void setModelRotationMode(RotationMode newMode) { modelRotationMode = newMode; }

    /**
     * @brief Gets the selected model index.
     * 
     * @return The current selected model index.
     */
    int getModelSelect() const { return modelSelect; }

    /**
     * @brief Gets the selected shader index.
     * 
     * @return The current selected shader index.
     */
    int getShaderSelect() const { return shaderSelect; }

    /**
     * @brief Gets the flag indicating whether to show the grid.
     * 
     * @return True if the grid should be shown, false otherwise.
     */
    bool getShowGrid() const { return showGrid; }

    /**
     * @brief Gets the current model rotation mode.
     * 
     * @return The current rotation mode for the model.
     */
    RotationMode getModelRotationMode() const { return modelRotationMode; }
};
