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
		float width = ImGui::GetContentRegionAvail().x;
		float height = ImGui::GetContentRegionAvail().y;
		ImVec2 btnSize = ImVec2(height, height);

		//Center the next group if we know what the group size will be
		if (groupSize)
			ImGui::SameLine(width / 2 - groupSize / 2);

		ImGui::BeginGroup();

		if (ImGui::Button("Save", btnSize))
		{
			//TODO:
			//Save the project
		}
		ImGui::SameLine();
		if (ImGui::Button("Play", btnSize))
		{
			//TODO:
			//Cache scene state
			//Enter play mode

			//Unless already playing then:
			//Exit play mode
			//Reset game state
		}
		ImGui::SameLine();
		if (ImGui::Button("Compile", btnSize))
		{
			//TODO:
			//Build game module
		}
		ImGui::SameLine();
		if (ImGui::Button("Hot\nSwap", btnSize))
		{
			//TODO: 
			//Hotswap game module (easy function call)
			//Reload scene data (could be complex, do research on best efficient approach)
		}
		ImGui::SameLine();
		if (ImGui::Button("NR\nHot\nSwap", btnSize))
		{
			//TODO:
			//Only hotswap game module (Non Reload, means dont reload scene data)
		}

		ImGui::EndGroup();
		groupSize = ImGui::GetItemRectSize().x;
	}
}
