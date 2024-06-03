#pragma once
#include <glm/glm.hpp>
#include <string>
class Surface
{

public:
    std::string name; 
    glm::vec3 position;
    glm::vec3 color;
    float radius;
    float roughness;

};

