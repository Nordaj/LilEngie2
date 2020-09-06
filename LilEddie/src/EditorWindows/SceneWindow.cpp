#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Graphics/DebugDrawing.h>
#include "../Components/SceneCameraComponent.h"
#include "../WindowManager.h"
#include "LilTreeWindow.h"
#include "SceneWindow.h"

namespace LilEddie
{
	void SceneWindow::Init()
	{
		Subscribe(EventType::ExitPlayMode);
		Subscribe(EventType::EditorSceneRecall);

		gfx = game->renderer.gfx;
		dbg = &game->renderer.debugDrawing;

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

		treeWin = (LilTreeWindow*)manager->GetWindow("Lil Tree");
		trsGizmo.Init(game, dbg, camera);
	}

	void SceneWindow::OnDraw()
	{
		//Transform gizmos
		if (treeWin)
		{
			Actor* actor = game->sceneManager.scene->GetActor(treeWin->selected);
			if (actor)
				DrawTransformGizmos(actor);
		}

		//Handle scn camera stuff
		scnCam->hovered = ImGui::IsWindowHovered();
		camPos = scnCam->actor->transform->position;
		camRotation = scnCam->actor->transform->rotation;

		//Handle window resizing
		if (ImGui::GetContentRegionAvail().x != frameSize.x || ImGui::GetContentRegionAvail().y != frameSize.y)
		{
			frameSize = ImGui::GetContentRegionAvail();
			camera->ResizeFramebuffer(frameSize.x, frameSize.y);
			frameColor = gfx->GetFramebufferTexture(camera->framebuffer);
		}

		//Display but without moving controls while adding a little indent
		ImVec2 cursorPos = ImGui::GetCursorPos();

		ImGui::Image(game->renderer.gfx->ImGuiGetTex(frameColor), frameSize, uv0, uv1);
		frameRectMin = ImGui::GetItemRectMin();

		ImGui::SetCursorPos(ImVec2(cursorPos.x + 5, cursorPos.y + 5));

		//Draw overlay controls
		ImGui::Checkbox("World Mode", &trsGizmo.worldMode);
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
		t->rotation = camRotation;
		camera = sceneCamera->CreateComponent<CameraComponent>();
		camera->EnableDebugGraphics(true);
		camera->width = 1;
		camera->height = 1;
		camera->UseFramebuffer();
		scnCam = sceneCamera->CreateComponent<SceneCameraComponent>();

		//Get frame color
		frameColor = gfx->GetFramebufferTexture(camera->framebuffer);
	}

	void SceneWindow::DrawTransformGizmos(Actor* actor)
	{
		if (!actor || !actor->transform)
			return;

		//Figure clip space mouse pos in scene view
		vec3 mp = vec3(ImGui::GetMousePos().x, ImGui::GetMousePos().y, 0);
		vec3 frm = vec3(frameRectMin.x, frameRectMin.y, 0);
		mp -= frm;
		mp /= vec3(frameSize.x, frameSize.y, 1);
		mp = mp * 2 - 1;
		mp.y *= -1;
		mp.z = 0;

		trsGizmo.selected = actor;
		trsGizmo.Draw(mp);
	}
}
