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
		window.PollEvents();
		window.SwapBuffer();
	}

	bool* Application::GetKeyStates()
	{
		return window.GetKeyStates();
	}

	MouseState* Application::GetMouseState()
	{
		return window.GetMouseState();
	}

	void Application::SetMousePosition(int x, int y)
	{
		window.SetMousePosition(x, y);
	}
}
