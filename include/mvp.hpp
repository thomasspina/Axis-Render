#pragma once

#include <glm/glm.hpp>
#include "camera.hpp"

class Mvp {
    public:
        Mvp();

        void updateMatrices(Camera& camera);
        void updateModel();
        void updateModel(Camera& camera);
        void updateView(Camera& camera);
        void updateProjection(Camera& camera);

        void updateModelYaw(float yoffset);
        void updateModelPitch(float xoffset);

        const glm::mat4 getModel() const;
        const glm::mat4 getView() const;
        const glm::mat4 getProjection() const;

    private:
        float modelYaw;
        float modelPitch;

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
};
