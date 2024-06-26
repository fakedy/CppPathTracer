
#include "UserInterface.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <memory>

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
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

}

void UserInterface::draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("DockSpace", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
    ImGuiID dockspace_id = ImGui::GetID("dockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
    ImGui::End();

    //ImGui::SetNextWindowPos(ImVec2(0, 0));
    //ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("ViewPort", NULL);
    viewPortData->width = ImGui::GetContentRegionAvail().x;
    viewPortData->height = ImGui::GetContentRegionAvail().y;
    ImGui::Image((void*)(intptr_t)viewPortData->textureID, ImVec2(viewPortData->width, viewPortData->height), ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();

    UserInterface::settingsPanel();

    UserInterface::sceneViewer();

    UserInterface::objectPanel();
    
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

void UserInterface::settingsPanel()
{
    ImGui::Begin("Settings");
    ImGui::Text("Frame time: %f ms", viewPortData->frameTime.count());
    UserInterface::frameTimeGraph();
    ImGui::Text("Frame rate: %f fps", 1000 / viewPortData->frameTime.count());
    ImGui::Text("Frame count: %i", viewPortData->frameCount);
    ImGui::Text("Rendering resolution: %ix%i ", viewPortData->image_width, viewPortData->image_height);
    ImGui::Text("Device: CPU");
    if (ImGui::SliderInt("Bounces", &viewPortData->bounces, 1, 100)) {
        viewPortData->shouldReset = true;
    }

    if (ImGui::SliderFloat("Gamma", &viewPortData->gammaValue, 0.001, 10)) {
        viewPortData->shouldReset = true;
    }

    if (ImGui::Checkbox("SSAA", &viewPortData->SSAA)) {
        viewPortData->shouldReset = true;
    }

    if (ImGui::Button("Reset")) {
        viewPortData->shouldReset = true;
    }

    ImGui::End();
}

void UserInterface::frameTimeGraph()
{
    static float values[90] = { 0 };
    static int values_offset = 0;
    static double refresh_time = 0.0;
    if (refresh_time == 0.0)
        refresh_time = ImGui::GetTime();
    while (refresh_time < ImGui::GetTime()) // Create dummy data at fixed 60 hz rate for the demo
    {
        values[values_offset] = viewPortData->frameTime.count();
        values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
        refresh_time += 1.0f / 60.0f;
    }
    ImGui::PlotLines("Frame time", values, IM_ARRAYSIZE(values), values_offset, NULL, 50.0f, 150.0f, ImVec2(0, 40));

    double avg = 0;
    for (int i = 0; i < IM_ARRAYSIZE(values); i++) {
        avg += values[i];
    }
    avg /= IM_ARRAYSIZE(values);
    ImGui::Text("Average frametime: ");
    ImGui::SameLine();
    ImGui::Text(std::to_string(avg).c_str());
}

void UserInterface::sceneViewer()
{
    ImGui::Begin("Scene", NULL, ImGuiWindowFlags_MenuBar);
    ImGui::BeginMenuBar();
    ImGui::Text("Scene");
    ImGui::EndMenuBar();

    float colorArray[3];
    float positionArray[3];

    for (size_t i = 0; i < viewPortData->scene->surfaces.size(); i++)
    {
        Surface& object = *viewPortData->scene->surfaces[i];
        if (ImGui::Button(("object" + std::to_string(i)).c_str())) {
            selectedObject = i;
        }
    }
    ImGui::End();
}

void UserInterface::objectPanel()
{
    ImGui::Begin("Object");

    float colorArray[3];
    float positionArray[3];
    Surface& object = *viewPortData->scene->surfaces[selectedObject];
    int materialIndex = object.materialIndex;
    std::shared_ptr<Material> material = viewPortData->scene->materials.at(materialIndex);

    ImGui::Text(object.name.c_str());

    if (ImGui::Button(("Material##" + std::to_string(selectedObject)).c_str())) {
        ImGui::OpenPopup(("Materials##" + std::to_string(selectedObject)).c_str());
    }

    ImGui::SameLine();
    ImGui::Text(("material: " + std::to_string(materialIndex)).c_str());
    if (ImGui::BeginPopup(("Materials##" + std::to_string(selectedObject)).c_str())) {
        ImGui::Text("Materials");
        ImGui::Separator;
        for (int i = 0; i < viewPortData->scene->materials.size(); i++) {
            if (ImGui::Selectable(std::to_string(i).c_str())) {
                object.materialIndex = i;
                viewPortData->shouldReset = true;
            }
        }
        ImGui::EndPopup();
    }


    colorArray[0] = material->albedo.r;
    colorArray[1] = material->albedo.g;
    colorArray[2] = material->albedo.b;

    if (ImGui::ColorEdit3(("Color##" + std::to_string(selectedObject)).c_str(), colorArray)) {    // because imgui use the label as id
        material->albedo = glm::vec3(colorArray[0], colorArray[1], colorArray[2]);     // we cant have the same label for each picker
        viewPortData->shouldReset = true;
    }

    positionArray[0] = object.position.x;
    positionArray[1] = object.position.y;
    positionArray[2] = object.position.z;
    if (ImGui::DragFloat3(("Translation##" + std::to_string(selectedObject)).c_str(), positionArray)) {
        object.position = glm::vec3(positionArray[0], positionArray[1], positionArray[2]);     // we cant have the same label for each picker
        viewPortData->shouldReset = true;
    }

    if (ImGui::SliderFloat(("Roughness##" + std::to_string(selectedObject)).c_str(), &material->roughness, 0, 1)) {
        viewPortData->shouldReset = true;
    }
    if (ImGui::SliderFloat(("Radius##" + std::to_string(selectedObject)).c_str(), &object.radius, 0, 100)) {
        viewPortData->shouldReset = true;
    }
    ImGui::Separator();



    ImGui::End();
}
