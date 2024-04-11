#include "App.h"
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <chrono>
#include <glm/glm.hpp>
#include <algorithm>


App::App()
{
    init();
}

App::~App()
{
}

void App::init()
{
    
    if (!glfwInit()) {
        std::cout << "GLFW FAILED TO INIT" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    window = glfwCreateWindow(1920, 1080, "Pathtracing", NULL, NULL);
    if (!window) {
        std::cout << "WINDOW CREATION FAILED" << std::endl;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD FAILED TO LOAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenBuffers(1, &PBO);



    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // measure time here
        auto startTime = std::chrono::high_resolution_clock::now();
        render();
        auto currentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> elapsed = currentTime - startTime;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("ViewPort");
        viewPortData->width = ImGui::GetContentRegionAvail().x;
        viewPortData->height = ImGui::GetContentRegionAvail().y;
        ImGui::Image((void*)(intptr_t)textureID, ImVec2(viewPortData->width, viewPortData->height), ImVec2(0,1), ImVec2(1,0));
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Begin("Settings");
        ImGui::Text("Render time: %f ms", elapsed.count());
        ImGui::End();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::render()
{
    if (!viewPortData->ImageData || viewPortData->image_width != viewPortData->width || viewPortData->image_height != viewPortData->height) {
        delete[]viewPortData->ImageData;
        viewPortData->ImageData = new uint32_t[viewPortData->width * viewPortData->height];

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewPortData->width, viewPortData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
        glBufferData(GL_PIXEL_UNPACK_BUFFER, viewPortData->width * viewPortData->height * 4, nullptr, GL_STREAM_DRAW);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        viewPortData->image_width = viewPortData->width;
        viewPortData->image_height = viewPortData->height;
    }

    for (uint32_t i = 0; i < viewPortData->width * viewPortData->height; i++) {
        viewPortData->ImageData[i] = raygen(i % viewPortData->width, i / viewPortData->height);
    }

    //glBindTexture(GL_TEXTURE_2D, viewPortData->textureID); // not needed as we only have one texture
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
    void* ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    if (ptr) {
        memcpy(ptr, viewPortData->ImageData, viewPortData->width * viewPortData->height*4);
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewPortData->width, viewPortData->height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

}


uint32_t App::raygen(int x, int y) {

    // lets cook
    
    // Normalized pixel coordinates
    glm::vec2 uv = glm::vec2(x / (float)viewPortData->width, y / (float)viewPortData->height);

    glm::vec4 color = glm::vec4(uv.x, uv.y, 0, 1.0);
    
    return convertColor(color);
}


uint32_t App::convertColor(glm::vec4& color) {
    color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

    uint8_t r = static_cast<uint8_t>(color.r * 255.0f);
    uint8_t g = static_cast<uint8_t>(color.g * 255.0f);
    uint8_t b = static_cast<uint8_t>(color.b * 255.0f);
    uint8_t a = static_cast<uint8_t>(color.a * 255.0f);

    return ((a << 24) | (b << 16) | (g << 8) | r);
}