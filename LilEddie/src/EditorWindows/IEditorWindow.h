#pragma once

#include <LilEngie.h>

namespace LilEddie
{
	class IEditorWindow
	{
	public:
		Game* game;
		bool isOpen = true;

	public:
		IEditorWindow(Game* game)
			: game(game) { }

		virtual void Init() { }
		virtual void OnDraw() { }
		virtual std::string WindowTitle() = 0;
	};
}

#define WINDOW_TITLE(n) public: std::string WindowTitle() override { return n; }
