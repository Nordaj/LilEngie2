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

		//Not responsible for calling Init, needs to be done after
		template <class T>
		bool RegisterWindow();

		IEditorWindow* GetWindow(const char* title);

	private:
		void MenuBar();
		void SaveAsPopup();
		void OpenScenePopup();
	};

	template <class T>
	inline bool WindowManager::RegisterWindow()
	{
		//Make sure its a valid type
		if (!std::is_base_of<IEditorWindow, T>())
			return false;

		//Create and add window
		IEditorWindow* win = new T(game);
		windows.push_back(win);
		win->manager = this;

		//Success
		return true;
	}
}
