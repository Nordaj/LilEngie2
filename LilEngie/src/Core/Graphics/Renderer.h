#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	class IGraphics;
	struct WinProp;
	enum GraphicsAPI;

	class LIL_API Renderer
	{
	private:
		IGraphics* gfx;
		float clearColor[4] = { 1, 0, 0.5f, 1 };

	public:
		Renderer();
		~Renderer();

		void Init(const WinProp &windowProperties, GraphicsAPI api);
		void Shutdown();

		void SetClearColor(float r, float g, float b, float a);
		void Render();
	};
}
