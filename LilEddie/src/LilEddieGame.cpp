#include <LilEngie.h>
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
#include "LilEddieGame.h"

namespace LilEddie
{
	LilEddieGame::LilEddieGame()
	{
		//Make game use our own component factory
		componentFactory = new ComponentFactory();
		componentFactory->InitComponentList();
		ComponentFactory::core = componentFactory;

		gamePath = "../Sandbox/";
	}

	LilEddieGame::~LilEddieGame()
	{
		for (IEditorWindow* win : windows)
			delete win;
	}

	void LilEddieGame::Init()
	{
		//Sloppy solution to start maximized, only temp
		application.window.Maximize();

		//Create all windows
		windows.push_back(new LilTreeWindow(this));

		PropertiesWindow* propertiesWindow = new PropertiesWindow(this);
		propertiesWindow->treeWindow = (LilTreeWindow*)windows[0];
		windows.push_back(propertiesWindow);

		windows.push_back(new GameWindow(this));
		windows.push_back(new SceneWindow(this));

		//Use imgui style
		SetupImGuiStyle();
	}

	void LilEddieGame::Start()
	{
		LIL_LOG("Hello, welcome to Lil Engie's editor - Lil Eddie");

		for (int i = 0; i < windows.size(); i++)
			windows[i]->Init();
	}

	void LilEddieGame::Update()
	{
		sceneManager.scene->DispatchActorEvent(ActorEvent::EditorUpdate);

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
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "CTRL+S"))
					sceneManager.SaveScene();
				if (ImGui::MenuItem("Save As", "CTRL+SHIFT+S")) {} //TODO
				if (ImGui::MenuItem("New", "CTRL+N")) {} //TODO
				if (ImGui::MenuItem("Reload", "CTRL+R")) {} //TODO

				ImGui::EndMenu();
			}
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
