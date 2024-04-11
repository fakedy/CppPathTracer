#include "App.h"
#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <chrono>


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

    glGenTextures(1, &viewPortData->textureID);
    glBindTexture(GL_TEXTURE_2D, viewPortData->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
        



        // to here

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("ViewPort");
        viewPortData->width = ImGui::GetContentRegionAvail().x;
        viewPortData->height = ImGui::GetContentRegionAvail().y;
        ImGui::Image((void*)(intptr_t)viewPortData->textureID, ImVec2(viewPortData->width, viewPortData->height));
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
    }

    for (uint32_t i = 0; i < viewPortData->width * viewPortData->height; i++) {
        viewPortData->ImageData[i] = 0xffff00ff;
    }

    glBindTexture(GL_TEXTURE_2D, viewPortData->textureID); // not needed as we only have one texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewPortData->width, viewPortData->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, viewPortData->ImageData);
    viewPortData->image_width = viewPortData->width;
    viewPortData->image_height = viewPortData->height;
}


void raygen(int x, int y) {

}
