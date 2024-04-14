#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include <chrono>
#include "ViewPortData.h"

class PathTracer
{
public:

    PathTracer(ViewPortData* viewPortData, Camera* camera);

    void render();

    ViewPortData* viewPortData;


private:

    Camera* camera;

    GLuint PBO;

    void init();

    glm::vec3 raygen(uint32_t x, uint32_t y);
    uint32_t convertColor(const glm::vec4& color);
};

