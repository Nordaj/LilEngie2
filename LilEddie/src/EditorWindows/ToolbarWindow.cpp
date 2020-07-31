#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include "ToolbarWindow.h"

namespace LilEddie
{
	void ToolbarWindow::Init()
	{

	}

	void ToolbarWindow::OnDraw()
	{
		float height = ImGui::GetContentRegionAvail().y;
		float width = ImGui::GetContentRegionAvail().x;
		ImVec2 btnSize = ImVec2(height, height);

		//Center the next group if we know what the group size will be
		if (groupSize)
			ImGui::SameLine(width / 2 - groupSize / 2);

		ImGui::BeginGroup();

		if (ImGui::Button("Save", btnSize))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Play", btnSize))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Compile", btnSize))
		{

		}

		ImGui::EndGroup();
		groupSize = ImGui::GetItemRectSize().x;
	}
}
