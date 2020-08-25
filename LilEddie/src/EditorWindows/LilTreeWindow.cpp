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

		//Add actor button
		if (ImGui::Button("New Actor", ImVec2(ImGui::GetWindowWidth() - 20, 25)))
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

		//Parent swap
		if (dragChild && dragParent)
		{
			dragChild->ChangeParent(dragParent);
			dragParent = dragChild = nullptr;
		}
	}

	void LilTreeWindow::DrawActorList(Actor* actor)
	{
		//Make sure its supposed to show
		if (actor->hideInTree)
			return;

		//No padding
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 3));

		//Setup default flags
		ImGuiTreeNodeFlags flags = 0;
		flags |= ImGuiTreeNodeFlags_DefaultOpen;
		flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
		flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_FramePadding;
		flags |= ImGuiTreeNodeFlags_SpanFullWidth;

		//Draw as selected if selected
		if (actor->uid == selected)
			flags |= ImGuiTreeNodeFlags_Selected;

		//Draw with bullet if no children
		if (actor->GetChildrenCount() == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;

		//Draw this actor node (in order for reasonable drag drop graphics, need to implement my own tree node, or use another method)
		bool tnOpen = ImGui::TreeNodeEx(actor->name.c_str(), flags);

		//Drag drop functionality
		if (ImGui::BeginDragDropSource())
		{
			ImGui::Text(actor->name.c_str());
			ImGui::SetDragDropPayload("TreeActorNode", &actor, sizeof(Actor*));
			ImGui::EndDragDropSource();
		}
		else if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* pl = ImGui::AcceptDragDropPayload("TreeActorNode");
			if (pl)
			{
				dragParent = actor;
				dragChild = *(Actor**)pl->Data;
			}

			ImGui::EndDragDropTarget();
		}

		//Draw sub actors
		if (tnOpen)
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

		ImGui::PopStyleVar(2);
	}
}
