#include "App.h"
#include "Window.h"
#include "PathTracer.h"
#include "UserInterface.h"
#include "ViewPortData.h"
#include "KeyHandler.h"
#include <iostream>
#include <chrono>

void renderData();
Window* window = new Window();
ViewPortData* viewPortData = new ViewPortData(1920, 1080); // width, height
Camera* camera = new Camera(glm::vec3(0,0,6), 45, 1920, 1080); // pos, fov, width, height
PathTracer* pathTracer = new PathTracer(viewPortData, camera);
UserInterface* UI = new UserInterface(viewPortData);

float deltaTime = 0;
float lastFrame = 0;


App::App()
{
    KeyHandler::setKeyCallback(window->getWindow());
    
    UI->init(window->getWindow());


    while (!window->windowShouldClose()) { // main loop

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        update(deltaTime); // logic
        UI->draw();
        window->update(renderData); // main render call
        
    }
    UI->shutdown();
    window->shutdown();
}


float movSpeed = 5.0f; // :l
float degree = 0.0f;
float distance = 10.0f;

void App::update(float deltaTime) {


    if (KeyHandler::getKeyDown(KeyHandler::A)) {
        //camera->cameraPos += glm::vec3(-movSpeed, 0, 0) * deltaTime;
        degree = degree + movSpeed * deltaTime;
        camera->cameraPos.x = glm::cos(degree) * distance;
        camera->cameraPos.z = glm::sin(degree) * distance;
        pathTracer->update();
    }
    else if (KeyHandler::getKeyDown(KeyHandler::D)) {
        //camera->cameraPos += glm::vec3(movSpeed, 0, 0) * deltaTime;
        degree = degree - movSpeed*deltaTime;
        camera->cameraPos.x = glm::cos(degree) * distance;
        camera->cameraPos.z = glm::sin(degree) * distance;
        pathTracer->update();
    }

    if (KeyHandler::getKeyDown(KeyHandler::W)) {
        //camera->cameraPos += glm::vec3(0, 0, -movSpeed) * deltaTime;
        distance = distance - movSpeed * deltaTime;
        camera->cameraPos.x = glm::cos(degree) * distance;
        camera->cameraPos.z = glm::sin(degree) * distance;
        pathTracer->update();
    }
    else if (KeyHandler::getKeyDown(KeyHandler::S)) {
        //camera->cameraPos += glm::vec3(0, 0, movSpeed) * deltaTime;
        distance = distance + movSpeed * deltaTime;
        camera->cameraPos.x = glm::cos(degree) * distance;
        camera->cameraPos.z = glm::sin(degree) * distance;
        pathTracer->update();
    }
    
}


// function that gets thrown into window class to be used for rendering
void renderData() {
    pathTracer->render();
    UI->draw();
}


App::~App()
{
}

