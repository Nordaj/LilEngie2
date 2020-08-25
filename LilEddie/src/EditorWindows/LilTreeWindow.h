#pragma once

#include <string>
#include <LilEngie.h>
#include "IEditorWindow.h"

namespace LilEddie
{
	class LilTreeWindow : public IEditorWindow
	{
		WINDOW_TITLE("Lil Tree")

	public:
		std::string selected = "";

	private:
		SceneManager* scnMgr = nullptr;
		std::string newActorUid;
		std::string newActorName;
		bool renaming = false;
		Actor* dragChild = nullptr;
		Actor* dragParent = nullptr;

	public:
		LilTreeWindow(Game* game) 
			: IEditorWindow(game) {}

		void Init() override;
		void OnDraw() override;

	private:
		void DrawActorList(Actor* actor);
	};
}
