#include <string>
#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include "IEditorWindow.h"
#include "LilTreeWindow.h"

namespace LilEddie
{
	void LilTreeWindow::Init()
	{
		scnMgr = &game->sceneManager;

		newActorUid.resize(255);
		newActorName.resize(255);
	}

	void LilTreeWindow::OnDraw()
	{
		if (!scnMgr) return;
		Scene* s = scnMgr->scene;

		//Check for renaming (F2)
		if (game->input.GetKeyDown(Key::F2) && ImGui::IsWindowFocused() && selected != "")
			renaming = true;

		ImGui::Begin("LilTree");

		//Add actor button
		if (ImGui::Button("Add Actor", ImVec2(ImGui::GetWindowWidth() - 20, 25)))
		{
			ImGui::OpenPopup("Create Actor");
		}

		if (ImGui::BeginPopup("Create Actor"))
		{
			ImGui::InputText("UID", &newActorUid[0], newActorUid.size());
			ImGui::InputText("Name", &newActorName[0], newActorName.size());

			if (ImGui::Button("Create"))
			{
				//Create actor here
				Actor* a = s->CreateActor(newActorUid);
				a->name = newActorName;

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		//Scene tree
		DrawActorList(s->GetActor("ROOT"));

		ImGui::End();
	}

	void LilTreeWindow::DrawActorList(Actor* actor)
	{
		//Setup default flags
		ImGuiTreeNodeFlags flags = 0;
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
		flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;

		//Draw as selected if selected
		if (actor->uid == selected)
			flags |= ImGuiTreeNodeFlags_Selected;

		//Draw with bullet if no children
		if (actor->GetChildrenCount() == 0)
			flags |= ImGuiTreeNodeFlags_Bullet;

		//Draw this actor
		if (ImGui::TreeNodeEx(actor->name.c_str(), flags))
		{
			//Set as selected if clicked
			if (ImGui::IsItemClicked())
				selected = actor->uid;

			//Draw each child
			int count = actor->GetChildrenCount();
			for (int i = 0; i < count; i++)
				DrawActorList(actor->GetChild(i));

			ImGui::TreePop();
		}
	}
}
