#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
class App
{
public:
	App();
	~App();

private:

    GLuint textureID;
    GLuint PBO;

    struct ViewPortData {
        uint32_t width;
        uint32_t height;
        int image_width, image_height;
        uint32_t* ImageData = NULL;

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
    uint32_t raygen(int x, int y);
    uint32_t convertColor(glm::vec4& color);
};

