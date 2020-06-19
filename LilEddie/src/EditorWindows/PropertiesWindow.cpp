#include <string>
#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include "IEditorWindow.h"
#include "LilTreeWindow.h"
#include "PropertiesWindow.h"

namespace LilEddie
{
	void PropertiesWindow::Init()
	{
		scnMgr = &game->sceneManager;
	}

	void PropertiesWindow::OnDraw()
	{
		Actor* sa = scnMgr->scene->GetActor(treeWindow->selected);

		ImGui::Begin("Properties");

		if (sa)
		{
			//Just show transform for now, no clue how to display property data since its just functions
			if (sa->transform)
			{
				ImGui::DragFloat3("Position", &sa->transform->position[0], .1);
				ImGui::DragFloat3("Rotation", &sa->transform->euler[0], .1);
				ImGui::DragFloat3("Scale", &sa->transform->scale[0], .1);
			}
		}

		ImGui::End();
	}
}
