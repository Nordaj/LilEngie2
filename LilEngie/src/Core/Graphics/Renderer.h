#pragma once

#include <Core/EventSystem/Events.h>
#include <Core/Core.h>

namespace LilEngie
{
	class IGraphics;
	struct WinProp;
	enum class GraphicsAPI;

	class LIL_API Renderer : public IEventListener
	{
	public:
		static Renderer* core;
		IGraphics* gfx;

	private:
		float clearColor[4] = { 1, 0, 0.5f, 1 };

	public:
		Renderer();
		~Renderer();

		void Init(const WinProp &windowProperties, GraphicsAPI api);
		void Shutdown();

		void SetClearColor(float r, float g, float b, float a);
		void Render();

	private:
		void OnEvent(const Event &e) override;
	};
}
