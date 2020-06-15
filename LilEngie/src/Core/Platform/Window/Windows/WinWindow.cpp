#include <functional>
#include <Core/Debug/Log.h>
#include <Vendor/imgui/imgui_impl_win32.h>
#include "../MouseState.h"
#include "WinWindow.h"

#ifdef LIL_WINDOWS
#include <Windows.h>

//Manual forward declaration for imgui to avoid windows.h includion
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace LilEngie
{
	namespace WinWindow
	{
		//Public
		Event resizeEvent = Event(EventType::WindowResize);
		Event closeEvent = Event(EventType::WindowClose);
		MouseState WinWindow::mouseState = {};
		bool keyStates[255] = {};

		//Private
		HWND hwnd;
		HINSTANCE hInstance;
		HDC hdc;
		const char *className = "Lil Engie Window";

		LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		bool Init(const char *title, int width, int height, void** handle, void** devCtx)
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

			//Give the handle
			*handle = hwnd;
			*devCtx = hdc;

			//Register raw mouse input
			RAWINPUTDEVICE rid[1];
			rid[0].usUsagePage = 0x01;
			rid[0].usUsage = 0x02;
			rid[0].dwFlags = 0;
			rid[0].hwndTarget = 0;
			if (!RegisterRawInputDevices(rid, 1, sizeof(rid[0])))
				LIL_ERROR("Could not register raw mouse input...");
		}

		void Close()
		{
			DestroyWindow(hwnd);
		}

		void GetWindowPos(int* x, int* y)
		{
			RECT r;
			GetWindowRect(hwnd, &r);
			*x = r.left;
			*y = r.top;
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

		void SetMousePosition(int x, int y)
		{
			SetCursorPos(x, y);
		}

		void SetMouseVisibility(bool visible)
		{
			ShowCursor(visible);
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

		#ifdef LIL_ENABLE_IMGUI
			if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
				return true;
		#endif //LIL_ENABLE_IMGUI

			switch (msg)
			{
				case WM_PAINT:
					hdc = BeginPaint(hwnd, &ps);
					EndPaint(hwnd, &ps);
					break;
				case WM_DESTROY:
					EventManager::core->Dispatch(closeEvent);
					PostQuitMessage(0);
					break;
				case WM_SIZE:
					//This dispatches the resize event many times. Look into solving this.
					resizeEvent.args[0].asInt = LOWORD(lParam);
					resizeEvent.args[1].asInt = HIWORD(lParam);
					EventManager::core->Dispatch(resizeEvent);
					break;
				case WM_KEYDOWN:
					keyStates[(uint8)wParam] = true;
					break;
				case WM_KEYUP:
					keyStates[(uint8)wParam] = false;
					break;
				case WM_LBUTTONDOWN:
					mouseState.buttons[(uint)MouseButton::Left] = true;
					break;
				case WM_LBUTTONUP:
					mouseState.buttons[(uint)MouseButton::Left] = false;
					break;
				case WM_MBUTTONDOWN:
					mouseState.buttons[(uint)MouseButton::Middle] = true;
					break;
				case WM_MBUTTONUP:
					mouseState.buttons[(uint)MouseButton::Middle] = false;
					break;
				case WM_RBUTTONDOWN:
					mouseState.buttons[(uint)MouseButton::Right] = true;
					break;
				case WM_RBUTTONUP:
					mouseState.buttons[(uint)MouseButton::Right] = false;
					break;
				case WM_XBUTTONDOWN:
					if (HIWORD(wParam) == XBUTTON1)
						mouseState.buttons[(uint)MouseButton::XButton1] = true;
					if (HIWORD(wParam) == XBUTTON2)
						mouseState.buttons[(uint)MouseButton::XButton2] = true;
					break;
				case WM_XBUTTONUP:
					if (HIWORD(wParam) == XBUTTON1)
						mouseState.buttons[(uint)MouseButton::XButton1] = false;
					if (HIWORD(wParam) == XBUTTON2)
						mouseState.buttons[(uint)MouseButton::XButton2] = false;
					break;
				case WM_MOUSEWHEEL:
					mouseState.wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
					break;
				case WM_INPUT:
				{
					uint dwSize;
					GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
					LPBYTE lpb = new BYTE[dwSize]{ 0 };
					GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

					RAWINPUT* raw = (RAWINPUT*)lpb;
					if (raw->header.dwType == RIM_TYPEMOUSE)
					{
						mouseState.mouseX = raw->data.mouse.lLastX;
						mouseState.mouseY = raw->data.mouse.lLastY;
					}
					delete[] lpb;
					break;
				}
				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
					break;
			}

			return S_OK;
		}
	}
}

#endif //LIL_WINDOWS
