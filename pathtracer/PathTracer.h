#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include <chrono>
#include "ViewPortData.h"
#include "Ray.h"

class PathTracer
{
public:

    PathTracer(ViewPortData* viewPortData, Camera* camera);

    void render();

    ViewPortData* viewPortData;

    struct Sphere {
        glm::vec3 position;
        glm::vec3 color;
        float radius;
        float roughness;
    };

private:


    struct PayLoad {
        glm::vec3 normal;
        float hitDistance;
        glm::vec3 hitPosition;
        Sphere* sphere;
    };

    Camera* camera;

    GLuint PBO;

    void init();
    void resize();
    void bufferData();
    

    std::vector<glm::vec4> accumilated_image;
    std::vector<uint32_t> widthIterator, heightIterator;

    PayLoad traceRay(Ray ray);
    PayLoad closestHit(Ray ray, float hitDistance, Sphere* closestSphere);
    PayLoad miss(Ray ray);

    float getRandFloat(float lower, float upper);

    glm::vec3 raygen(uint32_t x, uint32_t y);
    uint32_t convertColor(const glm::vec4& color);
};

