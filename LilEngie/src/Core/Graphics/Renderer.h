#pragma once

#include <queue>
#include <Core/EventSystem/Events.h>
#include <Core/Math/LilMath.h>
#include <Core/Core.h>
#include "Camera.h"

namespace LilEngie
{
	class IRenderable;
	class IGraphics;
	class ICBuffer;
	class Game;
	struct WinProp;
	enum class GraphicsAPI;

	class LIL_API Renderer : public IEventListener
	{
	public:
		IFramebuffer* framebuffer = nullptr;
		IGraphics* gfx;
		ICBuffer* cbPerObject;
		ICBuffer* cbPerCamera;
		std::vector<Camera*> cameras;
		float aspectRatio = 1.77777f;

	private:
		float clearColor[4] = { .2f, .2f, .2f, 1 };
		std::queue<IRenderable*> opaqueQueue;
		bool isClosing = false;
		Game* game;

	public:
		Renderer();
		~Renderer();

		void Init(const WinProp& windowProperties, GraphicsAPI api, Game* game);
		void Shutdown();

		void UseFramebuffer();
		void ResizeFramebuffer(int w, int h);

		void SetClearColor(float r, float g, float b, float a);
		void QueueOpaque(IRenderable* renderable);
		void Render();

	private:
		void OnEvent(const Event &e) override;
		void RenderAllCameras();

		void InitImGui(const WinProp& windowProperties);
	};
}
