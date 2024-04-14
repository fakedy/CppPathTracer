
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
    ImGui::Text("Frame time: %f fps", 1000 / viewPortData->elapsed.count());
    ImGui::Text("Frame count: %i", 1); // TODO frame count
    ImGui::Text("Rendering resolution: %ix%i ", viewPortData->image_width, viewPortData->image_height);
    
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
