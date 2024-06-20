#include "Box.h"



float Box::intersection(Ray ray) const
{
    // TODO, this is currently for a sphere not a box
    glm::vec3 rayDir = ray.direction;

    glm::vec3 cameraPos = ray.origin - position;
    // Equations to calculate hit on a sphere.
    float a = glm::dot(rayDir, rayDir);
    float b = 2.0f * dot(cameraPos, rayDir);
    float c = dot(cameraPos, cameraPos) - (radius * radius);
    float disc = b * b - 4.0f * a * c;

    if (disc < 0) {
        return -1;
    }

    float t = ((-b - sqrt(disc)) / (2.0f * a));
    return t;
}