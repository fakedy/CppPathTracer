#include "PathTracer.h"
#include <vector>
#include <random>
#include <iostream>

std::vector<PathTracer::Sphere> spheres;

PathTracer::PathTracer(ViewPortData* viewPortData, Camera* camera)
{
    this->viewPortData = viewPortData;
    this->camera = camera;
    init();
}



void PathTracer::init()
{
    glGenTextures(1, &viewPortData->textureID);
    glBindTexture(GL_TEXTURE_2D, viewPortData->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenBuffers(1, &PBO);

    Sphere sphere1;
    Sphere sphere2;
    Sphere sphere3;


    sphere1.position = glm::vec3(0, 0, -3);
    sphere1.color = glm::vec3(1, 0, 1);
    sphere1.radius = 0.5f;
    sphere1.roughness = 0.3f;

    sphere2.position = glm::vec3(-1, 1, -1);
    sphere2.color = glm::vec3(1, 1, 0);
    sphere2.radius = 0.5f;
    sphere2.roughness = 0.3f;

    sphere3.position = glm::vec3(2, -1, 0);
    sphere3.color = glm::vec3(0, 0, 1);
    sphere3.radius = 1.5f;
    sphere3.roughness = 0.3f;


    spheres.push_back(sphere1);
    spheres.push_back(sphere2);
    spheres.push_back(sphere3);

}

void PathTracer::render()
{
    auto startTime = std::chrono::high_resolution_clock::now();
    // If we need to resize the viewport window
    if (!viewPortData->ImageData || viewPortData->image_width != viewPortData->width || viewPortData->image_height != viewPortData->height) {
        resize();
    }

    for (uint32_t y = 0; y < viewPortData->height; y++) {
        for (uint32_t x = 0; x < viewPortData->width; x++) {
            viewPortData->ImageData[x + y * viewPortData->width] = convertColor(glm::clamp(glm::vec4(raygen(x, y), 1.0f), 0.0f, 1.0f));
        }
    }

    bufferData();

    auto currentTime = std::chrono::high_resolution_clock::now();

    viewPortData->elapsed = currentTime - startTime;
}

void PathTracer::resize()
{
    delete[]viewPortData->ImageData;
    viewPortData->ImageData = new uint32_t[viewPortData->width * viewPortData->height];

    glBindTexture(GL_TEXTURE_2D, viewPortData->textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewPortData->width, viewPortData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, viewPortData->width * viewPortData->height * 4, nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    viewPortData->image_width = viewPortData->width;
    viewPortData->image_height = viewPortData->height;
    camera->resize(viewPortData->width, viewPortData->height);
}

void PathTracer::bufferData()
{
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
    glBufferSubData(GL_PIXEL_UNPACK_BUFFER, 0, viewPortData->width * viewPortData->height * 4, viewPortData->ImageData);

    glBindTexture(GL_TEXTURE_2D, viewPortData->textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewPortData->width, viewPortData->height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

PathTracer::PayLoad PathTracer::traceRay(Ray ray)
{
    float hitDistance = 1000000000;
    Sphere* closestSphere = nullptr;

    glm::vec3 rayDir = ray.direction;

    for (int i = 0; i < spheres.size(); i++) {

        glm::vec3 cameraPos = ray.origin - spheres[i].position;
        // Equations to calculate hit on a sphere.
        float a = dot(rayDir, rayDir);
        float b = 2.0f * dot(cameraPos, rayDir);
        float c = dot(cameraPos, cameraPos) - (spheres[i].radius * spheres[i].radius);
        float disc = b * b - 4.0f * a * c;

        if (disc < 0) {
            continue;
        }
  
        float t = ((-b - sqrt(disc)) / (2.0f * a));

        if (t > 0.0 && t < hitDistance) {
            hitDistance = t;
            closestSphere = &spheres[i];
        }

    }

    if (closestSphere == nullptr) {
        return miss(ray);
    }

    return closestHit(ray, hitDistance, closestSphere);
}

PathTracer::PayLoad PathTracer::closestHit(Ray ray, float hitDistance, Sphere* closestSphere)
{

    glm::vec3 cameraPos = ray.origin - closestSphere->position;

    PayLoad payLoad;

    payLoad.hitDistance = hitDistance;
    payLoad.hitPosition = cameraPos + ray.direction * hitDistance;
    payLoad.normal = glm::normalize(payLoad.hitPosition);
    payLoad.sphere = closestSphere;
    return payLoad;
}

PathTracer::PayLoad PathTracer::miss(Ray ray)
{
    PayLoad payLoad;
    payLoad.hitDistance = -1;
    return payLoad;
}

glm::vec3 PathTracer::raygen(uint32_t x, uint32_t y) {

    glm::vec3 cameraPos = camera->getPosition();
    glm::vec3 rayDir = camera->getDirections()[x + y * viewPortData->width];
    Ray ray;
    ray.origin = cameraPos;
    ray.direction = rayDir;

    glm::vec3 finalColor = glm::vec3(0, 0, 0);
    
    int maxBounce = 6;
    float energy = 1.0;

    for (int i = 0; i < maxBounce; i++) {
        PayLoad payLoad = traceRay(ray);
        if (payLoad.hitDistance < 0) { // if no hit
            finalColor += glm::vec3(0.1, 0.1, 0.1) * energy;
            break;
        }
        glm::vec3 lightDir = glm::vec3(-1.0, -1.0, -1.0);
        float lightIntensity = glm::max(dot(payLoad.normal, -lightDir), 0.0f);
        glm::vec3 sphereColor = payLoad.sphere->color * lightIntensity;
        finalColor += sphereColor * energy;
        energy *= 0.7;
        
        // might need to noramlize something
        ray.origin = payLoad.hitPosition + payLoad.normal * 0.0001f;
        glm::vec3 randVec = glm::vec3(
            1 - getRandFloat(0, 1) * payLoad.sphere->roughness,
            1 - getRandFloat(0, 1) * payLoad.sphere->roughness,
            1 - getRandFloat(0, 1) * payLoad.sphere->roughness);
        ray.direction = glm::normalize(glm::reflect(ray.direction, payLoad.normal*randVec));

    }
    
    return finalColor;
}

float PathTracer::getRandFloat(float lower, float upper) {
    float random = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (upper-lower)));
    return random;
}

uint32_t PathTracer::convertColor(const glm::vec4& color) {

    uint8_t r = static_cast<uint8_t>(color.r * 255.0f);
    uint8_t g = static_cast<uint8_t>(color.g * 255.0f);
    uint8_t b = static_cast<uint8_t>(color.b * 255.0f);
    uint8_t a = static_cast<uint8_t>(color.a * 255.0f);

    return ((a << 24) | (b << 16) | (g << 8) | r);
}