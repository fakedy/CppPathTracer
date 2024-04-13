#pragma once
#include "PathTracer.h"

class UserInterface
{

public:

	void init(GLFWwindow* window);

	void draw();

	void shutdown();

private:

	PathTracer* pathTracer;

};

