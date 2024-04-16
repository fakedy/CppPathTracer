#include "App.h"
#include <iostream>
#include "Window.h"
#include "PathTracer.h"
#include "UserInterface.h"
#include "ViewPortData.h"

void renderData();
Window* window = new Window();
ViewPortData* viewPortData = new ViewPortData(1920, 1080);
Camera* camera = new Camera(glm::vec3(0,0,6), 45, 1920, 1080);
PathTracer* pathTracer = new PathTracer(viewPortData, camera);
UserInterface* UI = new UserInterface(viewPortData);

App::App()
{
    UI->init(window->getWindow());
    while (!window->windowShouldClose()) { // main loop
        window->update(renderData);

        pathTracer->render();
        UI->draw();
    }
    UI->shutdown();
    window->shutdown();
}


void renderData() {
    pathTracer->render();
    UI->draw();
}


App::~App()
{
}

