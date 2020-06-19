#pragma once

#include <LilEngie.h>

namespace LilEddie
{
	class IEditorWindow
	{
	public:
		Game* game;

	public:
		IEditorWindow(Game* game)
			: game(game) { }

		virtual void Init() { }
		virtual void OnDraw() { }
	};
}
