#pragma once

#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include "IEditorWindow.h"

namespace LilEddie
{
	class GameWindow : public IEditorWindow
	{
		WINDOW_TITLE("Game View")

	private:
		IGraphics* gfx;
		ITexture* frameColor;
		ImVec2 frameSize;
		ImVec2 uv0;
		ImVec2 uv1;

	public:
		GameWindow(Game* game)
			: IEditorWindow(game) { }

		void Init() override;
		void OnDraw() override;
	};
}
