#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include "GameWindow.h"

namespace LilEddie
{
	void GameWindow::Init()
	{
		game->renderer.UseFramebuffer();
		gfx = game->renderer.gfx;

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

	void GameWindow::OnDraw()
	{
		ImGui::Begin("Game View");

		//Handle resizing
		if (ImGui::GetContentRegionAvail().x != frameSize.x || ImGui::GetContentRegionAvail().y != frameSize.y)
		{
			frameSize = ImGui::GetContentRegionAvail();
			game->renderer.ResizeFramebuffer(frameSize.x, frameSize.y);
			frameColor = gfx->GetFramebufferTexture(game->renderer.framebuffer);
		}

		//Draw image
		ImGui::Image(game->renderer.gfx->ImGuiGetTex(frameColor), frameSize, uv0, uv1);

		ImGui::End();
	}
}
