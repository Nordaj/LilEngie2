//LilEddie - LilEngie's Editor application entry point

#include <iostream>
#include <vector>
#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include "ImGuiStyle.h"
#include "EditorWindows/IEditorWindow.h"
#include "EditorWindows/LilTreeWindow.h"
#include "EditorWindows/PropertiesWindow.h"
#include "EditorWindows/SceneWindow.h"
#include "EditorWindows/GameWindow.h"

namespace LilEddie
{
	Game* game;
	std::vector<IEditorWindow*> windows;

	void Init()
	{
		game->application.window.Maximize();

		//Setup component creation factory ptr for the engine to use for serialization
		//game->sceneManager.gameComponentFactory = CreateComponentFromString;

		windows.push_back(new LilTreeWindow(game));

		PropertiesWindow* propertiesWindow = new PropertiesWindow(game);
		propertiesWindow->treeWindow = (LilTreeWindow*)windows[0];
		windows.push_back(propertiesWindow);

		windows.push_back(new GameWindow(game));
		windows.push_back(new SceneWindow(game));
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
		game->sceneManager.scene->DispatchActorEvent(ActorEvent::EditorUpdate);

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

		//Draw menu bar
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		if (ImGui::BeginMainMenuBar())
		{
			ImGui::PopStyleVar();
			if (ImGui::BeginMenu("Window"))
			{
				for (IEditorWindow* win : windows)
				{
					if (ImGui::MenuItem(win->WindowTitle().c_str()))
						win->isOpen = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		//Draw each window only if opened
		for (IEditorWindow* win : windows)
		{
			if (win->isOpen)
			{
				ImGui::Begin(win->WindowTitle().c_str(), &win->isOpen);
				win->OnDraw();
				ImGui::End();
			}
		}

		ImGui::End();
	}
}

int main()
{
	using namespace LilEddie;

	ComponentFactory* factory = new ComponentFactory();
	factory->InitComponentList();
	ComponentFactory::core = factory;

	game = new Game(Start, Update, Init);

	game->gamePath = "../Sandbox/"; //TODO make use path argument
	
	game->Run();
	delete game;
	delete factory;
	return 0;
}
