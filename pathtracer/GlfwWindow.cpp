#include "GlfwWindow.h"
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <chrono>
#include <glm/glm.hpp>
#include <algorithm>
#include "UserInterface.h"


UserInterface* UI = new UserInterface();

GlfwWindow::GlfwWindow()
{
    init();
}

void GlfwWindow::init()
{
    if (!glfwInit()) {
        std::cout << "GLFW FAILED TO INIT" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    window = glfwCreateWindow(1920, 1080, "Pathtracing", NULL, NULL);
    if (!window) {
        std::cout << "WINDOW CREATION FAILED" << std::endl;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD FAILED TO LOAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }


    // replace this and render somewhere else
    UI->init();
    while (!glfwWindowShouldClose(window)) {


        UI->draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

 
    UI->shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    // ----------------------------------------
}

bool GlfwWindow::windowShouldClose()
{
    return windowShouldClose();
}
