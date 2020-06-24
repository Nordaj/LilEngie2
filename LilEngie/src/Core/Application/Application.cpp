#include <memory>
#include <Core/Platform/Window/Window.h>
#include <Core/Platform/Window/MouseState.h>
#include "Application.h"

namespace LilEngie
{
	void Application::Init()
	{
		windowProperties = window.Init();
	}

	void Application::Update()
	{
		windowProperties.width = window.GetSizeX();
		windowProperties.height = window.GetSizeY();

		window.PollEvents();
		window.SwapBuffer();
	}
}
