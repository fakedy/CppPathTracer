#pragma once
#include <glad/glad.h>
#include <chrono>
#include "Scene.h"


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

        Scene* scene = new Scene();

        GLuint textureID; // viewport texture

        uint32_t width; // viewport size
        uint32_t height; 
        uint32_t image_width, image_height; // this is the size we render
        uint32_t* ImageData = NULL; // data we send to gpu and imgui

        uint32_t frameCount = 1;
        int bounces = 4;
        bool shouldReset = false;
        bool SSAA = true;
        float gammaValue = 2.2;
        // Starting to realise having this as just some data storage is inconvenient when wanting to make calls


    std::chrono::duration<double, std::milli> frameTime; // frame time
    std::chrono::duration<double, std::milli> elapsed; // frame time
};