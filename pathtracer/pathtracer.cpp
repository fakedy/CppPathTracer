#include "PathTracer.h"
#include <vector>
#include <random>
#include <iostream>
#include <execution>
#include "Surface.h"


PathTracer::PathTracer(ViewPortData* viewPortData, Camera* camera, Scene* scene)
{
    this->viewPortData = viewPortData;
    this->camera = camera;
    this->scene = scene;
    accumilated_image.resize(viewPortData->width*viewPortData->height);
    widthIterator.resize(viewPortData->width);
    heightIterator.resize(viewPortData->height);
    for (uint32_t i = 0; i < viewPortData->width; i++) {
        widthIterator[i] = i;
    }
    for (uint32_t i = 0; i < viewPortData->height; i++) {
        heightIterator[i] = i;
    }
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
    
}

void PathTracer::render()
{
    auto startTime = std::chrono::high_resolution_clock::now();
    // If we need to resize the viewport window
    if (!viewPortData->ImageData || viewPortData->image_width != viewPortData->width || viewPortData->image_height != viewPortData->height) {
        resize();
    }

    std::for_each(std::execution::par, heightIterator.begin(), heightIterator.end(), [this](uint32_t y) {
        for (uint32_t x = 0; x < viewPortData->width; x++) {
            glm::vec4 color = glm::vec4(raygen(x, y), 1.0f);
            accumilated_image[x + y * viewPortData->width] += color;
            glm::vec4 acc_color = accumilated_image[x + y * viewPortData->width];
            acc_color /= (float)viewPortData->frameCount;
            acc_color = glm::clamp(acc_color, glm::vec4(0.0f), glm::vec4(1.0f));
            viewPortData->ImageData[x + y * viewPortData->width] = convertColor(acc_color);
        }
    });


    viewPortData->frameCount++;
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
    
    viewPortData->frameCount = 1;
    accumilated_image.resize(viewPortData->width * viewPortData->height);
    accumilated_image.clear();
    widthIterator.resize(viewPortData->width);
    heightIterator.resize(viewPortData->height);
    for (uint32_t i = 0; i < viewPortData->width; i++) {
        widthIterator[i] = i;
    }
    for (uint32_t i = 0; i < viewPortData->height; i++) {
        heightIterator[i] = i;
    }
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
    float hitDistance = 1000000000; // high value to allow algorithm to find a min value
    Surface* closestSurface = nullptr;

    glm::vec3 rayDir = ray.direction;

    for (int i = 0; i < scene->surfaces.size(); i++) {

        glm::vec3 cameraPos = ray.origin - scene->surfaces[i].position;
        // Equations to calculate hit on a sphere.
        float a = dot(rayDir, rayDir);
        float b = 2.0f * dot(cameraPos, rayDir);
        float c = dot(cameraPos, cameraPos) - (scene->surfaces[i].radius * scene->surfaces[i].radius);
        float disc = b * b - 4.0f * a * c;

        if (disc < 0) { 
            continue;
        }
  
        float t = ((-b - sqrt(disc)) / (2.0f * a));

        if (t > 0.0 && t < hitDistance) {
            hitDistance = t;
            closestSurface = &scene->surfaces[i];
        }

    }

    if (closestSurface == nullptr) {
        return miss(ray);
    }

    return closestHit(ray, hitDistance, closestSurface);
}

PathTracer::PayLoad PathTracer::closestHit(Ray ray, float hitDistance, Surface* closestSurface)
{

    glm::vec3 cameraPos = ray.origin - closestSurface->position;

    PayLoad payLoad;
    payLoad.hitDistance = hitDistance;
    payLoad.hitPosition = cameraPos + ray.direction * hitDistance;
    payLoad.normal = glm::normalize(payLoad.hitPosition);
    payLoad.surface = closestSurface;
    payLoad.hitPosition += closestSurface->position;
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

    glm::vec3 finalColor(0.0f); // variable to store the accumilated color from bounces
    glm::vec3 lightDir = glm::vec3(-1.0, -1.0, -1.0); // scene light direction
    glm::vec3 backGroundColor = glm::vec3(0.6f, 0.7f, 0.9f); // background color of scene
    
    int maxBounce = 5;
    float energy = 1.0;

    for (int i = 0; i < maxBounce; i++) {
        PayLoad payLoad = traceRay(ray);
        if (payLoad.hitDistance < 0) { // If we dont hit anything
            finalColor += backGroundColor * energy;
            break;
        }
        float lightIntensity = glm::max(dot(payLoad.normal, -lightDir), 0.0f); // dot product between lightdir and the surface normal
        glm::vec3 sphereColor = payLoad.surface->color * lightIntensity;
        finalColor += sphereColor * energy;
        energy *= 0.5; // Energy decrease on each bounce. Random value and not accurate
        

        ray.origin = payLoad.hitPosition + payLoad.normal * 0.0001f; // where we hit the sphere + offset by normal dir to prevent hitting ourselves
        glm::vec3 randVec = glm::vec3(getRandFloat(-0.5f, 0.5f), getRandFloat(-0.5f, 0.5f), getRandFloat(-0.5f, 0.5f));
        ray.direction = glm::reflect(ray.direction, payLoad.normal + payLoad.surface->roughness * randVec);

    }
    
    return finalColor;
}


/**
 * @brief  Returns a random float between lower and upper bound.
 *
 * 
 *
 * @param  float:  lower bound
 * @param  float:  upper bound
 *
 * @return float between lower and upper bound
 */
float PathTracer::getRandFloat(float lower, float upper) {
    float random = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (upper-lower)));
    random = random + lower;
    return random;
}


/**
* @brief Converts vec4 values from 0 to 1 space to 0 to 255
* 
* @param  glm::vec4&:  color
* 
*/
uint32_t PathTracer::convertColor(const glm::vec4& color) {

    uint8_t r = static_cast<uint8_t>(color.r * 255.0f);
    uint8_t g = static_cast<uint8_t>(color.g * 255.0f);
    uint8_t b = static_cast<uint8_t>(color.b * 255.0f);
    uint8_t a = static_cast<uint8_t>(color.a * 255.0f);

    // bitshifts values into a uint32_t
    return ((a << 24) | (b << 16) | (g << 8) | r);
}