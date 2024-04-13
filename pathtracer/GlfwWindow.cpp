#include "GlfwWindow.h"
#include "UserInterface.h"
#include <iostream>

UserInterface* UI = new UserInterface();

GlfwWindow::GlfwWindow()
{
    init();
    UI->init(window);
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

}

bool GlfwWindow::windowShouldClose()
{
    return windowShouldClose();
}

void GlfwWindow::update()
{
    // not sure i should have opengl stuff here but i dunno
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    UI->draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void GlfwWindow::shutdown()
{
    UI->shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}





