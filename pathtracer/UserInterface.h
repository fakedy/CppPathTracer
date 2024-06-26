#pragma once
#include "PathTracer.h"
#include "ViewPortData.h"
#include <GLFW/glfw3.h>

class UserInterface
{

public:

	UserInterface(ViewPortData* viewPortData);

	void init(GLFWwindow* window);

	void draw();

	void shutdown();

	void onResize();

private:

	uint32_t selectedObject = 0;

	ViewPortData* viewPortData;

	void settingsPanel();

	void frameTimeGraph();

	void objectPanel();
	
	void sceneViewer();

};

