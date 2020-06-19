#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <LilEngie.h>
#include "IEditorWindow.h"

namespace LilEddie
{
	class LilTreeWindow;

	class PropertiesWindow : public IEditorWindow
	{
	public:
		LilTreeWindow* treeWindow = nullptr;

	private:
		SceneManager* scnMgr = nullptr;

	public:
		PropertiesWindow(Game* game)
			: IEditorWindow(game) { }

		void Init() override;
		void OnDraw() override;

	private:
		void DrawProperty(std::string name, json value);
		void DrawArrayProperty(std::string& name, json arr);
	};
}
