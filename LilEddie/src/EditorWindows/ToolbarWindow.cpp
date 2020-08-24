#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include "LilEddieGame.h"
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
		if (ImGui::Button(game->IsPlaying() ? "Stop" : "Play", btnSize))
		{
			//TODO:
			//Cache scene state
			//Enter play mode
			game->SetPlayMode(!game->IsPlaying());

			//Unless already playing then:
			//Exit play mode
			//Reset game state
		}
		ImGui::SameLine();
		if (ImGui::Button("Compile", btnSize))
		{
			//TODO:
			//Build game module
			int err = ((LilEddieGame*)game)->CompileGameDLL();

			if (!err)
				LIL_LOG("Game module compilation finished.");
			if (err) ///cant use else with logs lol
				LIL_LOG("Game module compilation failed with code ", err);
		}
		ImGui::SameLine();
		if (ImGui::Button("Load\nGame", btnSize))
		{
			//TODO: 
			//Hotswap game module (easy function call)
			//Reload scene data (could be complex, do research on best efficient approach)

			int err = ((LilEddieGame*)game)->ReloadGameDLL();

			if (!err)
				LIL_LOG("Game module hot swap finished.");
			if (err) ///cant use else with logs lol
				LIL_LOG("Game module hot swap failed with code ", err);
		}

		ImGui::EndGroup();
		groupSize = ImGui::GetItemRectSize().x;
	}
}
