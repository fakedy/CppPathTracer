#pragma once
#define GLM_FORCE_SSE2
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <string>
#include "Ray.h"

class Surface
{

public:
    std::string name; 
    glm::vec3 position;
    float radius;

    int materialIndex = 0;

    virtual float intersection(Ray ray) const = 0;



};

