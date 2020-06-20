#pragma once

#include <string>
#include <LilEngie.h>
#include "IEditorWindow.h"

namespace LilEddie
{
	class LilTreeWindow : public IEditorWindow
	{
	public:
		std::string selected = "";

	private:
		SceneManager* scnMgr = nullptr;
		std::string newActorUid;
		std::string newActorName;
		bool renaming = false;

	public:
		LilTreeWindow(Game* game) 
			: IEditorWindow(game) {}

		void Init() override;
		void OnDraw() override;

	private:
		void DrawActorList(Actor* actor);
	};
}
