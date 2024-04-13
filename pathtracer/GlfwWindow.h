#pragma once
#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Camera.h"
class GlfwWindow : public Window
{
public:

	GlfwWindow();
	void init() override;
	bool windowShouldClose() override;

private:

	GLFWwindow* window;

};

