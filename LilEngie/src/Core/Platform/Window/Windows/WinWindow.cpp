#include <functional>
#include "WinWindow.h"

#ifdef LIL_WINDOWS
#include <Windows.h>
#include <Core/Game/ServiceLocator.h>

namespace LilEngie
{
	namespace WinWindow
	{
		//Public
		Event resizeEvent = Event(EventType::WindowResize);
		Event closeEvent = Event(EventType::WindowClose);

		//Private
		HWND hwnd;
		HINSTANCE hInstance;
		HDC hdc;
		const char *className = "Lil Engie Window";

		LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		bool Init(const char *title, int width, int height)
		{
			//Setup window class struct
			WNDCLASSEX wcex = {};
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wcex.lpfnWndProc = WindowProc;
			wcex.hInstance = hInstance;
			wcex.lpszClassName = className;
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);

			//Register class name
			if (!RegisterClassEx(&wcex))
				return false;

			//Create window
			hwnd = CreateWindow(
				className,
				title,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				width,
				height,
				NULL,
				NULL,
				hInstance,
				NULL
			);
			if (!hwnd)
				return false;

			//Get device context
			hdc = GetDC(hwnd);

			//Describe pixel format
			PIXELFORMATDESCRIPTOR pfd = {};
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cDepthBits = 24;
			pfd.cStencilBits = 8;
			pfd.cAuxBuffers = 0;
			pfd.iLayerType = PFD_MAIN_PLANE;

			//Setup pixel format
			int pf = ChoosePixelFormat(hdc, &pfd);
			if (pf == 0)
				return false;
			if (SetPixelFormat(hdc, pf, &pfd) == FALSE)
				return false;
			DescribePixelFormat(hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

			//Show the window
			ShowWindow(hwnd, SW_SHOW);
			UpdateWindow(hwnd);
		}

		void Close()
		{
			DestroyWindow(hwnd);

			SERVICES_GET(EventManager)->Dispatch(closeEvent);
		}

		void SetSize(int x, int y)
		{
			SetWindowPos(hwnd, NULL, NULL, NULL, x, y, SWP_NOMOVE | SWP_NOZORDER);
		}

		void GetSize(int *x, int *y)
		{
			RECT r = {};
			GetWindowRect(hwnd, &r);

			if (x != nullptr) *x = r.top - r.left;
			if (y != nullptr) *y = r.bottom - r.top;
		}

		void SetTitle(const std::string &title)
		{
			SetWindowTextA(hwnd, title.c_str());
		}

		void PollEvents()
		{
			MSG msg;
			while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					Close();

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		void SwapBuffer()
		{
			SwapBuffers(hdc);
		}

		LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			PAINTSTRUCT ps;
			HDC hdc;

			switch (msg)
			{
				case WM_PAINT:
					hdc = BeginPaint(hwnd, &ps);
					EndPaint(hwnd, &ps);
					break;
				case WM_DESTROY:
					PostQuitMessage(0);
					break;
				case WM_SIZE:
					//This dispatches the resize event many times. Look into solving this.
					resizeEvent.args[0].asInt = LOWORD(lParam);
					resizeEvent.args[1].asInt = HIWORD(lParam);
					SERVICES_GET(EventManager)->Dispatch(resizeEvent);
					break;
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
					break;
			}

			return S_OK;
		}
	}
}

#endif //LIL_WINDOWS
