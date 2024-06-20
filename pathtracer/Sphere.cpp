#include "Sphere.h"
#include <iostream>




float Sphere::intersection(Ray ray) const
{
    glm::vec3 rayDir = ray.direction;

    glm::vec3 cameraPos = ray.origin - position;
    // Equations to calculate hit on a sphere.
    float a = 1.0f; //glm::dot(rayDir, rayDir); // because rayDir should be normalized
    //std::cout << a << std::endl;
    float b = 2.0f * glm::dot(cameraPos, rayDir);
    float c = glm::dot(cameraPos, cameraPos) - (radius * radius);
    float disc = b * b - 4.0f * a * c;

    if (disc < 0) {
        return -1;
    }

    float t = ((-b - sqrt(disc)) / (2.0f * a));
	return t;
}
