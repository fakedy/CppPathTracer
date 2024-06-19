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

	ViewPortData* viewPortData;

	void settingsPanel();

	void objectPanel();
	
	void sceneViewer();

};

