#pragma once

#include <functional>
#include <Core/Core.h>
#include <Core/Platform/Window/Window.h>

namespace LilEngie
{
	class LIL_API Application
	{
	private:
		Window window;

	public:
		void Init();
		void Update();
	};
}
