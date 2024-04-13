#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include <chrono>

class PathTracer
{
public:


    void render();
    GLuint textureID;

    struct ViewPortData {
        uint32_t width;
        uint32_t height;
        uint32_t image_width, image_height;
        uint32_t* ImageData = NULL;

        ViewPortData(uint32_t width, uint32_t height) {
            this->width = width;
            this->height = height;
            this->image_height = width;
            this->image_height = height;
            this->ImageData = new uint32_t[width * height];
        }
    };

    ViewPortData* viewPortData = new ViewPortData(1920, 1080);

    std::chrono::duration<double, std::milli> elapsed;

private:

    Camera* camera;

    GLuint PBO;

    void init();

    glm::vec3 raygen(uint32_t x, uint32_t y);
    uint32_t convertColor(const glm::vec4& color);
};

