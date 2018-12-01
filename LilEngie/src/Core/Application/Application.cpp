#include <memory>
#include <Core/Platform/Window/Window.h>
#include "Application.h"

namespace LilEngie
{
	void Application::Init()
	{
		window.Init();
	}

	void Application::Update()
	{
		window.PollEvents();
		window.SwapBuffer();
	}
}
