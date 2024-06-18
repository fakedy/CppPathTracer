#include "App.h"
#include "Window.h"
#include "PathTracer.h"
#include "UserInterface.h"
#include "ViewPortData.h"
#include "KeyHandler.h"
#include <iostream>

void renderData();
Window* window = new Window();
ViewPortData* viewPortData = new ViewPortData(1920, 1080); // width, height
Camera* camera = new Camera(glm::vec3(0,0,6), 45, 1920, 1080); // pos, fov, width, height
PathTracer* pathTracer = new PathTracer(viewPortData, camera);
UserInterface* UI = new UserInterface(viewPortData);

App::App()
{
    KeyHandler::setKeyCallback(window->getWindow());
    
    UI->init(window->getWindow());
    while (!window->windowShouldClose()) { // main loop

        update(); // logic
        window->update(renderData); // main render call
        pathTracer->render();
        UI->draw();
        

    }
    UI->shutdown();
    window->shutdown();
}

void App::update() {


    if (KeyHandler::getKeyDown(KeyHandler::A)) {
        camera->cameraPos += glm::vec3(-0.8, 0, 0);
        pathTracer->update();
        camera->update();
    }
    else if (KeyHandler::getKeyDown(KeyHandler::D)) {
        camera->cameraPos += glm::vec3(0.8, 0, 0);
        pathTracer->update();
        camera->update();
    }
    if (KeyHandler::getKeyDown(KeyHandler::W)) {
        camera->cameraPos += glm::vec3(0, 0, -0.8);
        pathTracer->update();
        camera->update();
    }
    else if (KeyHandler::getKeyDown(KeyHandler::S)) {
        camera->cameraPos += glm::vec3(0, 0, 0.8);
        pathTracer->update();
        camera->update();
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

