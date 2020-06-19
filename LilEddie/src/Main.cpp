//LilEddie - LilEngie's Editor application entry point

#include <vector>
#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include "ImGuiStyle.h"
#include "EditorWindows/IEditorWindow.h"
#include "EditorWindows/LilTreeWindow.h"
#include "EditorWindows/PropertiesWindow.h"

namespace LilEddie
{
	Game* game;
	std::vector<IEditorWindow*> windows;

	void Init()
	{
		//Setup component creation factory ptr for the engine to use for serialization
		//game->sceneManager.gameComponentFactory = CreateComponentFromString;

		windows.push_back(new LilTreeWindow(game));

		PropertiesWindow* propertiesWindow = new PropertiesWindow(game);
		propertiesWindow->treeWindow = (LilTreeWindow*)windows[0];
		windows.push_back(propertiesWindow);
	}

	void Start()
	{
		LIL_LOG("Hello, welcome to Lil Engie 2.");

		SetupImGuiStyle();

		for (int i = 0; i < windows.size(); i++)
			windows[i]->Init();
	}

	void Update()
	{
		ImGui::ShowDemoWindow();

		for (int i = 0; i < windows.size(); i++)
			windows[i]->OnDraw();
	}
}

int main()
{
	using namespace LilEddie;

	game = new Game(Start, Update, Init);
	game->gamePath = "../Sandbox/"; //TODO make use path argument
	game->Run();
	delete game;
	return 0;
}
