#pragma once

#include <string>
#include "WindowManager.h"
#include <LilEngie.h>

namespace LilEddie
{
	class IEditorWindow;

	class LilEddieGame : public Game
	{
	public:
		std::string projName;

	private:
		void* gameLib = nullptr;
		WindowManager windowManager;

	public:
		LilEddieGame();
		~LilEddieGame();

		void ReloadGameDLL();

	protected:
		void Init() override;
		void Start() override;
		void Update() override;
	};
}
