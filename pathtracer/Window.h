#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>

class Window
{
public:

	Window();
	bool windowShouldClose();
	void update(std::function<void()> drawFunction);
	void shutdown();
	GLFWwindow* getWindow();


private:

	void init();
	GLFWwindow* window;



};

