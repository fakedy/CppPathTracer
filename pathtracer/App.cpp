#include "App.h"
#include <iostream>
#include "Window.h"
#include "PathTracer.h"
#include "UserInterface.h"
#include "ViewPortData.h"

void renderData();
Window* window = new Window();
ViewPortData* viewPortData = new ViewPortData(1920, 1080); // width, height
Camera* camera = new Camera(glm::vec3(0,0,6), 45, 1920, 1080); // pos, fov, width, height
PathTracer* pathTracer = new PathTracer(viewPortData, camera);
UserInterface* UI = new UserInterface(viewPortData);

bool moved = true;

App::App()
{
    UI->init(window->getWindow());
    while (!window->windowShouldClose()) { // main loop

        window->update(renderData); // main render call
        pathTracer->render();

        if (moved == true) {
            pathTracer->update();
            camera->update();
            moved = false;
        }

        UI->draw();
    }
    UI->shutdown();
    window->shutdown();
}


// function that gets thrown into window class to be used for rendering
void renderData() {
    pathTracer->render();
    UI->draw();
}


App::~App()
{
}

