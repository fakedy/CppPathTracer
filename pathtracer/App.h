#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class App
{
public:
	App();
	~App();

private:

    struct ViewPortData {
        int width;
        int height;
        int image_width, image_height;
        uint32_t* ImageData = NULL;
        GLuint textureID;

        ViewPortData(int width, int height) {
            this->width = width;
            this->height = height;
            this->image_height = width;
            this->image_height = height;
            this->ImageData = new uint32_t[width * height];
        }
    };

    ViewPortData* viewPortData = new ViewPortData(1920, 1080);

	GLFWwindow* window;



	void init();
    void render();
};

