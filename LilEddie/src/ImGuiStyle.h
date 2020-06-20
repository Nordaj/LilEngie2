#pragma once

#include <Vendor/imgui/imgui.h>

ImVec4 operator*(ImVec4 a, float b)
{
	return ImVec4(a.x * b, a.y * b, a.z * b, a.w);
}

inline void SetupImGuiStyle()
{
	//Colors
	ImVec4 white = ImVec4(1, 1, 1, 1);

	ImVec4 text = white;
	ImVec4 bg = white * .1;
	ImVec4 fg = white * .5;
	ImVec4 highlight = ImVec4(1, .25, .5, 1);

	//Styling
	ImGuiStyle& s = ImGui::GetStyle();
	s.WindowRounding = 0;
	s.FrameRounding = 5;
	s.ScrollbarRounding = 0;
	s.Alpha = 1;

	//Style colors
	s.Colors[ImGuiCol_Text] = text;
	s.Colors[ImGuiCol_TextDisabled] = text * .5;

    s.Colors[ImGuiCol_CheckMark] = fg;
    s.Colors[ImGuiCol_SliderGrab] = fg;
    s.Colors[ImGuiCol_SliderGrabActive] = white;

    s.Colors[ImGuiCol_WindowBg] = bg;
    s.Colors[ImGuiCol_ChildBg] = bg;
    s.Colors[ImGuiCol_PopupBg] = bg;
	s.Colors[ImGuiCol_MenuBarBg] = bg;

    s.Colors[ImGuiCol_FrameBg] = fg * .3;
    s.Colors[ImGuiCol_FrameBgHovered] = highlight * .3;
    s.Colors[ImGuiCol_FrameBgActive] = fg * .3;

    s.Colors[ImGuiCol_Border] = highlight;
    //s.Colors[ImGuiCol_BorderShadow] = highlight; //Keep default

	s.Colors[ImGuiCol_TitleBg] = highlight;
	s.Colors[ImGuiCol_TitleBgCollapsed] = highlight;
	s.Colors[ImGuiCol_TitleBgActive] = highlight;

	s.Colors[ImGuiCol_ScrollbarBg] = bg;
	s.Colors[ImGuiCol_ScrollbarGrab] = fg * .5;
	s.Colors[ImGuiCol_ScrollbarGrabHovered] = fg * .6;
	s.Colors[ImGuiCol_ScrollbarGrabActive] = fg * .7;

    s.Colors[ImGuiCol_Button] = fg * .3;
    s.Colors[ImGuiCol_ButtonHovered] = fg * .5;
    s.Colors[ImGuiCol_ButtonActive] = fg * .3;

    s.Colors[ImGuiCol_Header] = fg * .4;
    s.Colors[ImGuiCol_HeaderHovered] = fg * .6;
    s.Colors[ImGuiCol_HeaderActive] = fg * .4;

    /*
        ImGuiCol_Separator,
        ImGuiCol_SeparatorHovered,
        ImGuiCol_SeparatorActive,
        ImGuiCol_ResizeGrip,
        ImGuiCol_ResizeGripHovered,
        ImGuiCol_ResizeGripActive,
        ImGuiCol_Tab,
        ImGuiCol_TabHovered,
        ImGuiCol_TabActive,
        ImGuiCol_TabUnfocused,
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
