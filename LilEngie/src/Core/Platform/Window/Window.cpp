#ifdef LIL_WINDOWS
#include "Windows/WinWindow.h"
#endif

#include <Core/EventSystem/Events.h>
#include "Window.h"

namespace LilEngie
{
	WinProp Window::Init()
	{
		Subscribe(EventType::WindowResize);

	#ifdef LIL_WINDOWS
		WinProp val = {};

		WinWindow::Init(title.c_str(), width, height, &val.hwnd);

		val.width = width;
		val.height = height;
		return val;
	#endif //LIL_WINDOWS
	}

	void Window::Close()
	{
	#ifdef LIL_WINDOWS
		WinWindow::Close();
	#endif //LIL_WINDOWS
	}

	void Window::SetWidth(int x)
	{
	#ifdef LIL_WINDOWS
		int y = 0;
		WinWindow::GetSize(nullptr, &y);
		WinWindow::SetSize(x, y);
	#endif //LIL_WINDOWS
	}

	void Window::SetHeight(int y)
	{
	#ifdef LIL_WINDOWS
		int x = 0;
		WinWindow::GetSize(&x, nullptr);
		WinWindow::SetSize(x, y);
	#endif //LIL_WINDOWS
	}

	void Window::SetTitle(const std::string &title)
	{
	#ifdef LIL_WINDOWS
		WinWindow::SetTitle(title);
	#endif //LIL_WINDOWS
	}

	void Window::PollEvents()
	{
	#ifdef LIL_WINDOWS
		WinWindow::PollEvents();
	#endif //LIL_WINDOWS
	}

	void Window::SwapBuffer()
	{
	#ifdef LIL_WINDOWS
		WinWindow::SwapBuffer();
	#endif //LIL_WINDOWS
	}

	void Window::OnEvent(const Event &e)
	{
		switch (e.type)
		{
			case EventType::WindowResize:
				width = e.args[0].asInt;
				height = e.args[1].asInt;
				break;
			default:
				break;
		}
	}
}
