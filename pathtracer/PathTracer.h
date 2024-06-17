#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include <chrono>
#include "ViewPortData.h"
#include "Ray.h"
#include "Scene.h"

class PathTracer
{
public:

    PathTracer(ViewPortData* viewPortData, Camera* camera);

    void render();
    void update(); // when moving camera and such

    ViewPortData* viewPortData;


private:


    struct PayLoad {
        glm::vec3 normal;
        float hitDistance;
        glm::vec3 hitPosition;
        Surface* surface;
    };

    Camera* camera;
    Scene* scene;

    GLuint PBO;

    void init();
    void resize();
    void bufferData();
    

    std::vector<glm::vec4> accumilated_image;
    std::vector<uint32_t> widthIterator, heightIterator;

    PayLoad traceRay(Ray ray);
    PayLoad closestHit(Ray ray, float hitDistance, Surface* closestSurface);
    PayLoad miss(Ray ray);

    float getRandFloat(float lower, float upper);

    glm::vec3 raygen(double x, double y);
    uint32_t convertColor(const glm::vec4& color);
};

