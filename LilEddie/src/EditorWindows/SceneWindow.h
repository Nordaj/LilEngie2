#pragma once

#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include "IEditorWindow.h"

namespace LilEddie
{
	class SceneWindow : public IEditorWindow
	{
	private:
		CameraComponent* camera;
		IGraphics* gfx;
		ITexture* frameColor;
		ImVec2 frameSize;
		ImVec2 uv0;
		ImVec2 uv1;

	public:
		SceneWindow(Game* game)
			: IEditorWindow(game) { }

		void Init() override;
		void OnDraw() override;
	};
}
