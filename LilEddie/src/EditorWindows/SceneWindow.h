#pragma once

#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include "../Components/SceneCameraComponent.h"
#include "IEditorWindow.h"

namespace LilEddie
{
	class LilTreeWindow;

	class SceneWindow : public IEditorWindow, public IEventListener
	{
		WINDOW_TITLE("Scene View")

	private:
		vec3 camPos = vec3(0, 0, 0);
		vec3 camEuler = vec3(0, 0, 0);
		CameraComponent* camera;
		SceneCameraComponent* scnCam;
		IGraphics* gfx;
		ITexture* frameColor;
		ImVec2 frameSize;
		ImVec2 frameRectMin;
		ImVec2 uv0;
		ImVec2 uv1;
		DebugDrawing* dbg;
		LilTreeWindow* treeWin;
		TransformGizmo trsGizmo;

	public:
		SceneWindow(Game* game)
			: IEditorWindow(game) { }

		void Init() override;
		void OnDraw() override;
		void Reload() override;

		void OnEvent(const Event& e) override;

	private:
		void SetupSceneCamera();
		void DrawTransformGizmos(Actor* selected);
	};
}
