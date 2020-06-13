#pragma once

#include <string>
#include <functional>
#include <Core/Core.h>
#include <Core/EventSystem/Events.h>
#include "Keys.h"

namespace LilEngie
{
	struct WinProp
	{
		short width;
		short height;

		//Platform specific
	#ifdef LIL_WINDOWS
		void* hwnd;
		void* hdc;
	#endif //LIL_WINDOWS
	};

	class LIL_API Window : public IEventListener
	{
	private:
		int width = 960;
		int height = 540;
		std::string title = "My Game";

	public:
		WinProp Init();
		void Close();

		void SetWidth(int x);
		void SetHeight(int y);
		void SetTitle(const std::string &title);

		void PollEvents();
		void SwapBuffer();

		inline int GetSizeX() const { return width; }
		inline int GetSizeY() const { return height; }

		bool GetKeyState(Key key);
		bool* GetKeyStates();

	private:
		void OnEvent(const Event &e) override;
	};
}
