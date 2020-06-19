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

	public:
		LilTreeWindow(Game* game) 
			: IEditorWindow(game) {}

		void Init() override;
		void OnDraw() override;

	private:
		void DrawActorList(Actor* actor);
	};
}
