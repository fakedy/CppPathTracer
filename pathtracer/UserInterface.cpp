#include "UserInterface.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// made for opengl/glfw



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
    if (pathTracer != NULL) {
        pathTracer->viewPortData->width = ImGui::GetContentRegionAvail().x;
        pathTracer->viewPortData->height = ImGui::GetContentRegionAvail().y;
        ImGui::Image((void*)(intptr_t)pathTracer->textureID, ImVec2(pathTracer->viewPortData->width, pathTracer->viewPortData->height), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Begin("Settings");
        ImGui::Text("Render time: %f ms", pathTracer->elapsed.count());
        ImGui::Text("Render time: %f fps", 1000 / pathTracer->elapsed.count());
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
