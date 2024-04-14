#pragma once
#include <glad/glad.h>
#include <chrono>

class ViewPortData 
{
public:

        ViewPortData(uint32_t width, uint32_t height) {
            this->width = width;
            this->height = height;
            this->image_height = width;
            this->image_height = height;
            this->ImageData = new uint32_t[width * height];
        }

    GLuint textureID;

        uint32_t width;
        uint32_t height;
        uint32_t image_width, image_height;
        uint32_t* ImageData = NULL;


    std::chrono::duration<double, std::milli> elapsed;
};