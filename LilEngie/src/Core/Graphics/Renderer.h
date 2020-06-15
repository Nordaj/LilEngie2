#pragma once

#include <queue>
#include <Core/EventSystem/Events.h>
#include <Core/Math/LilMath.h>
#include <Core/Core.h>

namespace LilEngie
{
	class IRenderable;
	class IGraphics;
	class ICBuffer;
	struct WinProp;
	enum class GraphicsAPI;

	class LIL_API Renderer : public IEventListener
	{
	public:
		static Renderer* core;
		IGraphics* gfx;
		ICBuffer* cbPerObject;
		ICBuffer* cbPerScene;

	private:
		float clearColor[4] = { 1, 0, 0.5f, 1 };
		std::queue<IRenderable*> opaqueQueue;
		bool isClosing = false;

	public:
		Renderer();
		~Renderer();

		void Init(const WinProp& windowProperties, GraphicsAPI api);
		void Shutdown();

		void SetClearColor(float r, float g, float b, float a);
		void QueueOpaque(IRenderable* renderable);
		void Render();

	private:
		void OnEvent(const Event &e) override;

		void InitImGui(const WinProp& windowProperties);
	};
}
