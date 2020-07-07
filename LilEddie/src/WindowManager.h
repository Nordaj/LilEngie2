#pragma once

#include <vector>
#include <LilEngie.h>

namespace LilEddie
{
	class IEditorWindow;

	class WindowManager
	{
	public:
		std::vector<IEditorWindow*> windows;
		Game* game;

	private:
		std::string saveScenePath;

	public:
		WindowManager(Game* game);
		~WindowManager();

		void Init();
		void Update();
		void ReloadWindows();

	private:
		void MenuBar();
		void SaveAsPopup();
		void OpenScenePopup();
	};
}
