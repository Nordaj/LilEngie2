#pragma once

#include <functional>
#include <Core/Core.h>
#include <Core/Platform/Window/Window.h>

namespace LilEngie
{
	class MouseState;

	class LIL_API Application
	{
	public:
		Window window;
		WinProp windowProperties;

		void Init();
		void Update();
	};
}
