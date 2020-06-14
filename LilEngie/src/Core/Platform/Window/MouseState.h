#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	enum class MouseButton
	{
		Left = 0,
		Middle = 1,
		Right = 2,
		XButton1 = 3,
		XButton2 = 4
	};

	struct LIL_API MouseState
	{
		int mouseX;
		int mouseY;
		int wheelDelta;
		bool buttons[5] = {};
	};
}
