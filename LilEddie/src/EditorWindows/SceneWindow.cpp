#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include "../Components/SceneCameraComponent.h"
#include "SceneWindow.h"

namespace LilEddie
{
	void SceneWindow::Init()
	{
		Subscribe(EventType::ExitPlayMode);
		Subscribe(EventType::EditorSceneRecall);

		gfx = game->renderer.gfx;

		SetupSceneCamera();

		if (gfx->GetGraphicsAPI() == GraphicsAPI::OpenGL)
		{
			uv0 = ImVec2(0, 1);
			uv1 = ImVec2(1, 0);
		}
		else
		{
			uv0 = ImVec2(0, 0);
			uv1 = ImVec2(1, 1);
		}
	}

	void SceneWindow::OnDraw()
	{
		scnCam->hovered = ImGui::IsWindowHovered();
		camPos = scnCam->actor->transform->position;
		camEuler = scnCam->actor->transform->euler;

		//Handle window resizing
		if (ImGui::GetContentRegionAvail().x != frameSize.x || ImGui::GetContentRegionAvail().y != frameSize.y)
		{
			frameSize = ImGui::GetContentRegionAvail();
			camera->ResizeFramebuffer(frameSize.x, frameSize.y);
			frameColor = gfx->GetFramebufferTexture(camera->framebuffer);
		}

		ImGui::Image(game->renderer.gfx->ImGuiGetTex(frameColor), frameSize, uv0, uv1);
	}

	void SceneWindow::Reload()
	{
		SetupSceneCamera();
	}

	void SceneWindow::OnEvent(const Event& e)
	{
		switch (e.type)
		{
			case EventType::ExitPlayMode:
			case EventType::EditorSceneRecall:
				Reload();
				break;
			default:
				break;
		}
	}

	void SceneWindow::SetupSceneCamera()
	{
		//Reset frame size 
		frameSize = ImVec2(0, 0);

		//Create/Setup scene camera
		Actor* sceneCamera = game->sceneManager.scene->CreateActor("__LilEddieSceneCamera");
		sceneCamera->name = "__LilEddieSceneCamera";
		sceneCamera->serialize = false;
		sceneCamera->hideInTree = true;
		TransformComponent* t = sceneCamera->CreateComponent<TransformComponent>();
		t->position = camPos;
		t->euler = camEuler;
		camera = sceneCamera->CreateComponent<CameraComponent>();
		camera->EnableDebugGraphics(true);
		camera->width = 1;
		camera->height = 1;
		camera->UseFramebuffer();
		scnCam = sceneCamera->CreateComponent<SceneCameraComponent>();

		//Get frame color
		frameColor = gfx->GetFramebufferTexture(camera->framebuffer);
	}
}
