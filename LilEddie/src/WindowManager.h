#pragma once

#include <vector>
#include <LilEngie.h>

namespace LilEddie
{
	class IEditorWindow;

	class WindowManager
	{
	public:
		std::vector<IEditorWindow*> windows;
		Game* game;

	public:
		WindowManager(Game* game);
		~WindowManager();

		void Init();
		void Update();
	};
}
