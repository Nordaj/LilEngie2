#ifdef LIL_WINDOWS
#include "Windows/WinWindow.h"
#endif

#include <Core/EventSystem/Events.h>
#include "Keys.h"
#include "Window.h"

namespace LilEngie
{
	WinProp Window::Init()
	{
		Subscribe(EventType::WindowResize);

	#ifdef LIL_WINDOWS
		WinProp val = {};

		WinWindow::Init(title.c_str(), width, height, &val.hwnd, &val.hdc);

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

	void Window::SetMousePosition(int x, int y)
	{
	#ifdef LIL_WINDOWS
		WinWindow::SetMousePosition(x, y);
	#endif //LIL_WINDOWS
	}

	void Window::GetMousePosition(int* x, int* y)
	{
	#ifdef LIL_WINDOWS
		WinWindow::GetMousePosition(x, y);
	#endif //LIL_WINDOWS
	}

	void Window::SetMouseVisibility(bool visible)
	{
	#ifdef LIL_WINDOWS
		WinWindow::SetMouseVisibility(visible);
	#endif //LIL_WINDOWS
	}

	void Window::Maximize()
	{
		maximized = true;

	#ifdef LIL_WINDOWS
		WinWindow::Maximize();
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

	void Window::GetWindowPos(int* x, int* y)
	{
	#ifdef LIL_WINDOWS
		WinWindow::GetWindowPos(x, y);
	#endif //LIL_WINDOWS
	}

	bool Window::GetKeyState(Key key)
	{
	#ifdef LIL_WINDOWS
		return WinWindow::keyStates[(uint8)key];
	#endif //LIL_WINDOWS
	}

	bool* Window::GetKeyStates()
	{
	#ifdef LIL_WINDOWS
		return WinWindow::keyStates;
	#endif //LIL_WINDOWS
	}

	MouseState* Window::GetMouseState()
	{
	#ifdef LIL_WINDOWS
		return &WinWindow::mouseState;
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
