#include <vector>
#include <Vendor/imgui/imgui.h>
#include <Vendor/imgui/imgui_stdlib.h>
#include <LilEngie.h>
#include "ImGuiStyle.h"
#include "EditorWindows/IEditorWindow.h"
#include "EditorWindows/LilTreeWindow.h"
#include "EditorWindows/PropertiesWindow.h"
#include "EditorWindows/SceneWindow.h"
#include "EditorWindows/GameWindow.h"
#include "EditorWindows/ToolbarWindow.h"
#include "WindowManager.h"

namespace LilEddie
{
	WindowManager::WindowManager(Game* game)
		: game(game)
	{ }

	WindowManager::~WindowManager()
	{
		for (IEditorWindow* win : windows)
			delete win;
	}

	void WindowManager::Init()
	{
		//Create all windows
		windows.push_back(new LilTreeWindow(game));

		PropertiesWindow* propertiesWindow = new PropertiesWindow(game);
		propertiesWindow->treeWindow = (LilTreeWindow*)windows[0];
		windows.push_back(propertiesWindow);

		windows.push_back(new GameWindow(game));
		windows.push_back(new SceneWindow(game));
		windows.push_back(new ToolbarWindow(game));

		//Use imgui style
		SetupImGuiStyle();

		//Init all windows
		for (int i = 0; i < windows.size(); i++)
			windows[i]->Init();
	}

	void WindowManager::Update()
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

		//Draw menu bar
		MenuBar();

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

	void WindowManager::ReloadWindows()
	{
		for (IEditorWindow* win : windows)
			win->Reload();
	}

	void WindowManager::MenuBar()
	{
		bool openSaveAs = false;
		bool openSceneOpen = false;
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
		if (ImGui::BeginMainMenuBar())
		{
			ImGui::PopStyleVar();

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "CTRL+S"))
					game->sceneManager.SaveScene();
				if (ImGui::MenuItem("Save As", "CTRL+SHIFT+S"))
					openSaveAs = true;
				if (ImGui::MenuItem("Open", "CTRL+O"))
					openSceneOpen = true;
				if (ImGui::MenuItem("New", "CTRL+N"))
				{
					game->sceneManager.NewScene();
					ReloadWindows();
				}
				if (ImGui::MenuItem("Reload", "CTRL+R"))
				{
					std::string path = game->sceneManager.scene->path;
					game->sceneManager.LoadScene(path.c_str());
					ReloadWindows();
				}

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

		//Open popups outside of menu (odd imgui issue)
		if (openSaveAs)
		{
			ImGui::OpenPopup("Save Scene As");

			//Init save location
			std::string& path = game->sceneManager.scene->path;
			memcpy(&saveScenePath[0], &path[0], path.size());
		}

		if (openSceneOpen)
		{
			ImGui::OpenPopup("Open Scene");

			//Init save location
			std::string& path = game->sceneManager.scene->path;
			memcpy(&saveScenePath[0], &path[0], path.size());
		}

		SaveAsPopup();
		OpenScenePopup();
	}

	void WindowManager::SaveAsPopup()
	{
		ImGui::SetNextWindowSize(ImVec2(300, 100));
		if (ImGui::BeginPopupModal("Save Scene As"))
		{
			ImGui::InputText("path", &saveScenePath);

			if (ImGui::Button("Cancel"))
				ImGui::CloseCurrentPopup();

			ImGui::SameLine();

			if (ImGui::Button("Save"))
			{
				game->sceneManager.scene->path = saveScenePath;
				game->sceneManager.SaveScene();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void WindowManager::OpenScenePopup()
	{
		ImGui::SetNextWindowSize(ImVec2(300, 100));
		if (ImGui::BeginPopupModal("Open Scene"))
		{
			ImGui::InputText("path", &saveScenePath);

			if (ImGui::Button("Cancel"))
				ImGui::CloseCurrentPopup();

			ImGui::SameLine();

			if (ImGui::Button("Open"))
			{
				game->sceneManager.LoadScene(saveScenePath.c_str());
				ReloadWindows();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}
