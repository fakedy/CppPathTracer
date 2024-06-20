#include "PathTracer.h"
#include <vector>
#include <random>
#include <iostream>
#include <execution>
#include "Surface.h"
#include <algorithm>
#include "xoshiro.h"


double random_double(float lower, float upper);
double shiro_random_double(float lower, float upper);


PathTracer::PathTracer(ViewPortData* viewPortData, Camera* camera)
{
    this->viewPortData = viewPortData;
    this->camera = camera;
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
    startTime = std::chrono::high_resolution_clock::now();

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
    auto frameStartTime = std::chrono::high_resolution_clock::now();
    // If we need to resize the viewport window
    if (!viewPortData->ImageData || viewPortData->image_width != viewPortData->width || viewPortData->image_height != viewPortData->height) {
        resize();
    }
    if (viewPortData->shouldReset == true) {
        update();
        viewPortData->shouldReset = false;
    }

    if (viewPortData->frameCount < 2000){ // this is just a pause preventing cpu from heating my room up when idle
        std::for_each(std::execution::par, heightIterator.begin(), heightIterator.end(), [this](uint32_t y) {
            for (uint32_t x = 0; x < viewPortData->width; x++) {
            glm::vec4 color = glm::vec4(0.0f);

                if (viewPortData->SSAA == true) {
                    // SSAA 4X
                    for (int i = 0; i < 8; i++) {
                        color += glm::vec4(raygen(x + shiro_random_double(-0.5f, 0.5f), y + shiro_random_double(-0.5f, 0.5f)), 1.0f);
                    }

                    color /= 8;
                }
                else {
                    color = glm::vec4(raygen(x, y), 1.0f);
                }

                accumilated_image[x + y * viewPortData->width] += color;
                glm::vec4 acc_color = accumilated_image[x + y * viewPortData->width];
                acc_color /= (float)viewPortData->frameCount;
                acc_color = glm::clamp(acc_color, glm::vec4(0.0f), glm::vec4(1.0f));
                viewPortData->ImageData[x + y * viewPortData->width] = convertColor(acc_color); // store our color in the output in 255 range
            }
        });
        bufferData();
        
    }


    viewPortData->frameCount++; // increment framecount

    auto currentTime = std::chrono::high_resolution_clock::now();

    viewPortData->frameTime = currentTime - frameStartTime; // calculate the time a frame took to render
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

// not every frame but for when we move etc
void PathTracer::update() {
    viewPortData->frameCount = 1;
    accumilated_image.resize(viewPortData->width * viewPortData->height);
    accumilated_image.clear();
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
    float hitDistance = 20000; // view distance
    Surface* closestSurface = nullptr;

    for (auto& surfacePtr : viewPortData->scene->surfaces) {

        Surface& surface = *surfacePtr;

        
        float hitdist = surface.intersection(ray);

        if (hitdist > 0.0 && hitdist < hitDistance) {
            hitDistance = hitdist;
            closestSurface = &surface;
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

glm::vec3 PathTracer::raygen(double x, double y) {

    // clean this stuff up soon
    // too much duplicate code in SSAA if statement

    glm::vec3 light(0.0f); // variable to store the accumilated color from bounces
    glm::vec3 backGroundColor = glm::vec3(0.0f, 0.0f, 0.0f); // background color of scene

    glm::vec3 rayDir = camera->calcDirection(x, y);
    glm::vec3 cameraPos = camera->getPosition();

    Ray ray; // general ray
    Ray shadowRay; // ray info for calculating shadows
    ray.origin = cameraPos;
    ray.direction = glm::normalize(rayDir);
    glm::vec3 energy = glm::vec3(1.0f);

    for (int i = 0; i < viewPortData->bounces; i++) {
        PayLoad payLoad = traceRay(ray);

        if (payLoad.hitDistance < 0) { // If we dont hit anything
            light += backGroundColor * energy;
            break;
        }

        ray.origin = payLoad.hitPosition + payLoad.normal * 0.0001f; // where we hit the surface + offset by normal dir to prevent hitting ourselves
        glm::vec3 randVec = glm::vec3(shiro_random_double(-1.0f, 1.0f), shiro_random_double(-1.0f, 1.0f), shiro_random_double(-1.0f, 1.0f));
        // while the vector is outside the unit sphere generate new til its not
        while (glm::length2(randVec) > 1) {
            randVec = glm::vec3(shiro_random_double(-1.0f, 1.0f), shiro_random_double(-1.0f, 1.0f), shiro_random_double(-1.0f, 1.0f));
        }
        
        
        randVec = glm::normalize(randVec); // create unit vector
        if (glm::dot(randVec, payLoad.normal) < 0) { // if dot product is negative its pointing in wrong direction
            randVec = -randVec;
        }

        const Material material = *viewPortData->scene->materials[payLoad.surface->materialIndex];
        glm::vec3 perturbedNormal = (payLoad.normal + material.roughness * randVec);
        ray.direction = glm::normalize(glm::reflect(ray.direction, perturbedNormal));


        light += material.getEmission() * energy;
        energy *= material.albedo;
    }
    
    return light;
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
double random_double(float lower, float upper) {
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(lower, upper);
    return distribution(generator);
}

double shiro_random_double(float lower, float upper) {
    thread_local Xoshiro shiro = Xoshiro();
    return shiro.random_double(lower, upper);
}

/**
* @brief Converts vec4 values from 0 to 1 space to 0 to 255
* 
* @param  glm::vec4&:  color
* 
*/
uint32_t PathTracer::convertColor(const glm::vec4& color) {

    // gamma correction approx sqrt(x) or (glm::pow(color.r, 1 / gammaValue)

    float gammaValue = viewPortData->gammaValue;
    uint8_t r = static_cast<uint8_t>(glm::pow(color.r, 1 / gammaValue) * 255.0f);
    uint8_t g = static_cast<uint8_t>(glm::pow(color.g, 1 / gammaValue) * 255.0f);
    uint8_t b = static_cast<uint8_t>(glm::pow(color.b, 1 / gammaValue) * 255.0f);
    uint8_t a = static_cast<uint8_t>(color.a * 255.0f);

    // bitshifts values into a uint32_t
    return ((a << 24) | (b << 16) | (g << 8) | r);
}
