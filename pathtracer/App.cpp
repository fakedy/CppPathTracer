#include "App.h"
#include <iostream>
#include "GlfwWindow.h"
#include "PathTracer.h"

PathTracer* pathTracer = new PathTracer();


App::App()
{
    Window* window = new GlfwWindow();
    while (!window->windowShouldClose()) { // main loop
        pathTracer->render();
        window->update();
    }
    window->shutdown();
}

App::~App()
{
}

