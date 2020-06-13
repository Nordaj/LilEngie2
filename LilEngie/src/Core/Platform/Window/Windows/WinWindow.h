#pragma once

#include <functional>
#include <string>
#include <Core/EventSystem/Events.h>

namespace LilEngie
{
	namespace WinWindow
	{
		extern Event resizeEvent;
		extern Event closeEvent;

		extern bool keyStates[255];

		bool Init(const char *title, int width, int height, void** handle, void** hdc);
		void Close();

		void SetSize(int x, int y);
		void GetSize(int *x, int *y);
		void SetTitle(const std::string &title);

		void PollEvents();
		void SwapBuffer();
	}
}
