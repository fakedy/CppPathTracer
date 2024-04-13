#pragma once
#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class GlfwWindow : public Window
{
public:

	GlfwWindow();
	void init() override;
	bool windowShouldClose() override;
	void update() override;
	void shutdown() override;

private:

	GLFWwindow* window;

};

