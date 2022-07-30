#include "gui.h"

#include <imgui.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace holodeck;

void GUI::initialize(const Platform& platform)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) platform.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
}

void GUI::draw()
{
    ImGui::ShowDemoWindow();
}

void GUI::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    GUI::draw();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void GUI::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::use_mouse(bool use)
{
    ImGuiIO& io = ImGui::GetIO();

    if(use)
        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    else
        io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

bool GUI::wants_mouse()
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}