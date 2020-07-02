#pragma once

#include <Vendor/imgui/imgui.h>

inline ImVec4 operator*(ImVec4 a, float b)
{
    return ImVec4(a.x * b, a.y * b, a.z * b, a.w * b);
}

inline ImVec4 operator+(ImVec4 a, ImVec4 b)
{
    return ImVec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

//Try to use 0, .25, .5, .75, 1 (brightest)
inline ImVec4 Gradient(float x)
{
    ImVec4 a = ImVec4(255, 61, 145, 255);
    ImVec4 b = ImVec4(33, 29, 32, 255);

    a = a * (1 / 255.);
    b = b * (1 / 255.);

    return (b * (1. - x)) + (a * x);
}

inline void SetupImGuiStyle()
{
	//Colors
	ImVec4 white = ImVec4(1, 1, 1, 1);
	ImVec4 text = white;

	//Styling
	ImGuiStyle& s = ImGui::GetStyle();
	s.WindowRounding = 0;
	s.FrameRounding = 5;
	s.ScrollbarRounding = 0;
	s.Alpha = 1;

	//Style colors
	s.Colors[ImGuiCol_Text] = text;
	s.Colors[ImGuiCol_TextDisabled] = text * .5;

    s.Colors[ImGuiCol_CheckMark] = Gradient(1);
    s.Colors[ImGuiCol_SliderGrab] = Gradient(1);
    s.Colors[ImGuiCol_SliderGrabActive] = Gradient(.5);

    s.Colors[ImGuiCol_WindowBg] = Gradient(0);
    s.Colors[ImGuiCol_ChildBg] = Gradient(0);
    s.Colors[ImGuiCol_PopupBg] = Gradient(0);
	s.Colors[ImGuiCol_MenuBarBg] = Gradient(0);

    s.Colors[ImGuiCol_FrameBg] = Gradient(0) * 1.7;
    s.Colors[ImGuiCol_FrameBgHovered] = Gradient(.5);
    s.Colors[ImGuiCol_FrameBgActive] = Gradient(.25);

    s.Colors[ImGuiCol_Border] = Gradient(1);
    //s.Colors[ImGuiCol_BorderShadow] = highlight; //Keep default

    s.Colors[ImGuiCol_TitleBg] = Gradient(0);
	s.Colors[ImGuiCol_TitleBgCollapsed] = Gradient(0);
	s.Colors[ImGuiCol_TitleBgActive] = Gradient(.25);

    s.Colors[ImGuiCol_Tab] = Gradient(1);
    s.Colors[ImGuiCol_TabHovered] = Gradient(.75);
    s.Colors[ImGuiCol_TabActive] = Gradient(1);
    s.Colors[ImGuiCol_TabUnfocused] = Gradient(1);
    s.Colors[ImGuiCol_TabUnfocusedActive] = Gradient(1);

	s.Colors[ImGuiCol_ScrollbarBg] = Gradient(0);
	s.Colors[ImGuiCol_ScrollbarGrab] = Gradient(.5);
	s.Colors[ImGuiCol_ScrollbarGrabHovered] = Gradient(.75);
	s.Colors[ImGuiCol_ScrollbarGrabActive] = Gradient(.5);

    s.Colors[ImGuiCol_Button] = Gradient(0) * 1.7;
    s.Colors[ImGuiCol_ButtonHovered] = Gradient(.5);
    s.Colors[ImGuiCol_ButtonActive] = Gradient(.25);

    s.Colors[ImGuiCol_Header] = Gradient(.25);
    s.Colors[ImGuiCol_HeaderHovered] = Gradient(.5);
    s.Colors[ImGuiCol_HeaderActive] = Gradient(.25);

    s.Colors[ImGuiCol_Separator] = Gradient(.25);
    s.Colors[ImGuiCol_SeparatorHovered] = Gradient(.5);
    s.Colors[ImGuiCol_SeparatorActive] = Gradient(.25);

    /*
        ImGuiCol_ResizeGrip,
        ImGuiCol_ResizeGripHovered,
        ImGuiCol_ResizeGripActive,
        ImGuiCol_TabUnfocusedActive,
        ImGuiCol_PlotLines,
        ImGuiCol_PlotLinesHovered,
        ImGuiCol_PlotHistogram,
        ImGuiCol_PlotHistogramHovered,
        ImGuiCol_TextSelectedBg,
        ImGuiCol_DragDropTarget,
        ImGuiCol_NavHighlight,          // Gamepad/keyboard: current highlighted item
        ImGuiCol_NavWindowingHighlight, // Highlight window when using CTRL+TAB
        ImGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the CTRL+TAB window list, when active
        ImGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
        */
}
