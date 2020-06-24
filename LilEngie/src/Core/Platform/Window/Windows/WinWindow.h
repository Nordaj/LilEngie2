#pragma once

#include <functional>
#include <string>
#include <Core/EventSystem/Events.h>
#include "../MouseState.h"

namespace LilEngie
{
	namespace WinWindow
	{
		extern Event resizeEvent;
		extern Event closeEvent;

		extern MouseState mouseState;
		extern bool keyStates[255];

		bool Init(const char *title, int width, int height, void** handle, void** hdc);
		void Close();

		void GetWindowPos(int* x, int* y);
		void SetSize(int x, int y);
		void GetSize(int *x, int *y);
		void SetTitle(const std::string &title);

		void SetMousePosition(int x, int y);
		void GetMousePosition(int* x, int* y);
		void SetMouseVisibility(bool visible);

		void Maximize();

		void PollEvents();
		void SwapBuffer();
	}
}
