#include "gui.h"
#include "log.h"

#include <imgui.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <cmath>

using namespace holodeck;

void set_style();

void GUI::initialize(const Platform& platform)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    set_style();

    io.Fonts->AddFontFromFileTTF("contents/fonts/monogram.ttf", std::round(14 * platform.get_scale()));
    io.FontGlobalScale = 1.0f / platform.get_scale();
    
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) platform.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
}

void GUI::draw()
{
    ImGui::ShowDemoWindow();
    draw_main_menu();
}

void GUI::draw_main_menu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::BeginMenu("Change camera mode.."))
            {
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }    
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

bool GUI::wants_keyboard()
{
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureKeyboard;
}


void set_style()
{
    // ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.FrameRounding = 3.0f;
    style.ScrollbarRounding = 0;
    style.FrameBorderSize = 1.0f;
    style.WindowPadding = ImVec2(6.0f, 6.0f);
    style.FramePadding = ImVec2(5.0f, 3.0f);
    style.ItemSpacing = ImVec2(6.0f, 3.0f);
    style.TabBorderSize = 1.0f;
    style.WindowRounding = 6.0f;
    style.PopupRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.FramePadding = ImVec2(5.0f, 2.0f);

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.91f, 1.00f, 0.96f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.07f, 0.06f, 0.05f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.14f, 0.13f, 0.11f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.23f, 0.21f, 0.17f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.41f, 0.39f, 0.32f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.12f, 0.11f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.10f, 0.10f, 0.08f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.36f, 0.30f, 0.10f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.14f, 0.13f, 0.11f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.18f, 0.16f, 0.13f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.24f, 0.22f, 0.17f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.14f, 0.13f, 0.11f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.18f, 0.16f, 0.13f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.24f, 0.22f, 0.17f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.36f, 0.30f, 0.10f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.26f, 0.22f, 0.07f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.15f, 0.18f, 0.14f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.23f, 0.26f, 0.23f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.34f, 0.36f, 0.34f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.14f, 0.13f, 0.11f, 1.00f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.26f, 0.23f, 0.12f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.26f, 0.22f, 0.08f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.17f, 0.16f, 0.15f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.24f, 0.21f, 0.10f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.33f, 0.29f, 0.14f, 1.00f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.41f, 0.39f, 0.32f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}