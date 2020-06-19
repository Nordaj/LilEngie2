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
	}

	void LilTreeWindow::OnDraw()
	{
		if (!scnMgr) return;
		Scene* s = scnMgr->scene;

		ImGui::Begin("LilTree");

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
