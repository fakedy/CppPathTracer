#include "App.h"
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <chrono>
#include <glm/glm.hpp>
#include <algorithm>

#include "GlfwWindow.h"



App::App()
{
    //init();
    Window* window = new GlfwWindow();
    while (!window->windowShouldClose()) {
        
    }
}

App::~App()
{
}

void App::init()
{
    
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenBuffers(1, &PBO);

    camera = new Camera(glm::vec3(0, 0, 2), 45, viewPortData->width, viewPortData->height); // temp data


    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // RENDER -<-A<Z-D-AS-DAS-D-ASD
        auto startTime = std::chrono::high_resolution_clock::now();
        render();
        auto currentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> elapsed = currentTime - startTime;

    }
}

void App::render()
{
    // If we need to resize the viewport window
    if (!viewPortData->ImageData || viewPortData->image_width != viewPortData->width || viewPortData->image_height != viewPortData->height) {
        delete[]viewPortData->ImageData;
        viewPortData->ImageData = new uint32_t[viewPortData->width * viewPortData->height];

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewPortData->width, viewPortData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, viewPortData->width * viewPortData->height * 4, nullptr, GL_STREAM_DRAW);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        viewPortData->image_width = viewPortData->width;
        viewPortData->image_height = viewPortData->height;
        camera->resize(viewPortData->width, viewPortData->height);
    }


    for (uint32_t y = 0; y < viewPortData->height; y++) {
        for (uint32_t x = 0; x < viewPortData->width; x++) {
            viewPortData->ImageData[x + y * viewPortData->width] = convertColor(glm::clamp(glm::vec4(raygen(x, y), 1.0f), 0.0f, 1.0f));
        }
    }

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
    glBufferSubData(GL_PIXEL_UNPACK_BUFFER, 0, viewPortData->width * viewPortData->height * 4, viewPortData->ImageData);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewPortData->width, viewPortData->height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}



glm::vec3 App::raygen(uint32_t x, uint32_t y) {

    struct Sphere {
        glm::vec3 position;
        glm::vec4 color;
        float radius;
    };
    Sphere sphere1;

    sphere1.position = glm::vec3(0, 0, 0);
    sphere1.color = glm::vec4(1, 0, 1, 1);
    sphere1.radius = 0.5f;



    glm::vec3 cameraPos = camera->getPosition();
    cameraPos = cameraPos - sphere1.position;
    glm::vec3 rayDir = camera->getDirections()[x + y * viewPortData->width];

    // Equations to calculate hit on a sphere.
    float a = dot(rayDir, rayDir);
    float b = 2.0f * dot(cameraPos, rayDir);
    float c = dot(cameraPos, cameraPos) - (sphere1.radius * sphere1.radius);
    float disc = b * b - 4.0f * a * c;

    glm::vec3 color = glm::vec3(0, 0, 0);
    if (disc >= 0) {
        color = glm::vec3(1, 1, 0);
    }
    else {
        return color;
    }

    float t = ((-b - sqrt(disc)) / (2.0f * a));

    glm::vec3 hitPos = cameraPos + rayDir * t;
    glm::vec3 normal = normalize(hitPos);

    glm::vec3 lightDir = glm::vec3(-1.0, -1.0, -1.0);
    float lightIntensity = glm::max(dot(normal, -lightDir), 0.0f);
    color *= lightIntensity;

    return color;
}


uint32_t App::convertColor(const glm::vec4& color) {

    uint8_t r = static_cast<uint8_t>(color.r * 255.0f);
    uint8_t g = static_cast<uint8_t>(color.g * 255.0f);
    uint8_t b = static_cast<uint8_t>(color.b * 255.0f);
    uint8_t a = static_cast<uint8_t>(color.a * 255.0f);

    return ((a << 24) | (b << 16) | (g << 8) | r);
}