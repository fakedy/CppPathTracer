#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"


class Scene {
public:

    std::vector<Surface> surfaces;


    Scene() {
        Sphere sphere1;
        Sphere sphere2;
        Sphere sphere3;
        Sphere sphere4;
        Sphere sphere5;


        sphere1.position = glm::vec3(-2, 1, 0);
        sphere1.color = glm::vec3(1, 0, 1);
        sphere1.radius = 1.0f;
        sphere1.roughness = 0.2f;

        sphere2.position = glm::vec3(0, -101.5, 0);
        sphere2.color = glm::vec3(0.20, 0.25, 1);
        sphere2.radius = 100.0f;
        sphere2.roughness = 0.2f;

        sphere3.position = glm::vec3(2, -1, 0);
        sphere3.color = glm::vec3(0, 0, 0);
        sphere3.radius = 1.5f;
        sphere3.roughness = 0.02f;

        sphere4.position = glm::vec3(-1, -1, -9);
        sphere4.color = glm::vec3(0.8, 0.1, 0.1);
        sphere4.radius = 3.0f;
        sphere4.roughness = 0.2f;

        sphere5.position = glm::vec3(-1, -1.1, 0.1);
        sphere5.color = glm::vec3(0.1, 0.3, 0.5);
        sphere5.radius = 0.5f;
        sphere5.roughness = 0.2f;


        surfaces.push_back(sphere1);
        surfaces.push_back(sphere2);
        surfaces.push_back(sphere3);
        surfaces.push_back(sphere4);
        surfaces.push_back(sphere5);
    }
};

