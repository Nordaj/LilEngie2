//LilEddie - LilEngie's Editor application entry point

#include <vector>
#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include "ImGuiStyle.h"
#include "EditorWindows/IEditorWindow.h"
#include "EditorWindows/LilTreeWindow.h"
#include "EditorWindows/PropertiesWindow.h"
#include "EditorWindows/GameWindow.h"

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

		windows.push_back(new GameWindow(game));
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
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGuiWindowFlags winFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | 
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | 
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin("Dockspace Window", 0, winFlags);
		ImGui::PopStyleVar(3);

		ImGuiID dsid = ImGui::GetID("Dockspace");
		ImGui::DockSpace(dsid);

		ImGui::ShowDemoWindow();

		for (int i = 0; i < windows.size(); i++)
			windows[i]->OnDraw();

		ImGui::End();
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
