
#include "UserInterface.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// made for opengl/glfw

UserInterface::UserInterface(ViewPortData* viewPortData)
{
    this->viewPortData = viewPortData;
}

void UserInterface::init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

}

void UserInterface::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("ViewPort");
    viewPortData->width = ImGui::GetContentRegionAvail().x;
    viewPortData->height = ImGui::GetContentRegionAvail().y;
    ImGui::Image((void*)(intptr_t)viewPortData->textureID, ImVec2(viewPortData->width, viewPortData->height), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::Begin("Settings");
    ImGui::Text("Frame time: %f ms", viewPortData->elapsed.count());
    ImGui::Text("Frame rate: %f fps", 1000 / viewPortData->elapsed.count());
    ImGui::Text("Frame count: %i", viewPortData->frameCount);
    ImGui::Text("Rendering resolution: %ix%i ", viewPortData->image_width, viewPortData->image_height);
    ImGui::Text("Device: CPU");
    ImGui::SliderInt("Bounces", &viewPortData->bounces, 1, 4);

    if (ImGui::Checkbox("SSAA", &viewPortData->SSAA)) {
        viewPortData->shouldReset = true;
    }

    if(ImGui::Button("Reset")) {
        viewPortData->shouldReset = true;
    }

    ImGui::End();

    ImGui::Begin("Scene", NULL, ImGuiWindowFlags_MenuBar);
    ImGui::BeginMenuBar();
    ImGui::Text("Scene");
    ImGui::EndMenuBar();

    float colorArray[3];
    float positionArray[3];
    for (size_t i = 0; i < viewPortData->scene->surfaces.size(); i++)
    {
        Surface &object = viewPortData->scene->surfaces[i];

        ImGui::Text(object.name.c_str());

            colorArray[0] = object.color.r;
            colorArray[1] = object.color.g;
            colorArray[2] = object.color.b;
        if(ImGui::ColorEdit3(("Color##" + std::to_string(i)).c_str(), colorArray)) {    // because imgui use the label as id
            object.color = glm::vec3(colorArray[0], colorArray[1], colorArray[2]);     // we cant have the same label for each picker
            viewPortData->shouldReset = true;
        }

            positionArray[0] = object.position.x;
            positionArray[1] = object.position.y;
            positionArray[2] = object.position.z;
        if (ImGui::DragFloat3(("Translation##" + std::to_string(i)).c_str(), positionArray)) {
            object.position = glm::vec3(positionArray[0], positionArray[1], positionArray[2]);     // we cant have the same label for each picker
            viewPortData->shouldReset = true;
        }

        if (ImGui::SliderFloat(("Roughness##" + std::to_string(i)).c_str(), &object.roughness, 0, 1)) {
            viewPortData->shouldReset = true;
        }
        if (ImGui::SliderFloat(("Radius##" + std::to_string(i)).c_str(), &object.radius, 0, 100)) {
            viewPortData->shouldReset = true;
        }

    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UserInterface::onResize()
{

}
