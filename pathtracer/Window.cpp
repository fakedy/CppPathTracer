#include "Window.h"
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Window::Window()
{
    init();
}

void Window::init()
{
    if (!glfwInit()) {
        std::cout << "GLFW FAILED TO INIT" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    window = glfwCreateWindow(1920, 1080, "Pathtracer", NULL, NULL);
    if (!window) {
        std::cout << "WINDOW CREATION FAILED" << std::endl;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD FAILED TO LOAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

}


bool Window::windowShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::update(std::function<void()> drawFunction)
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawFunction();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::shutdown()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* Window::getWindow()
{
    return window;
}


// testing some simple input
// want to create some broadcast system for inputs that everyone can just listen to
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

}






