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
		std::string slnPath;

	private:
		void* gameLib = nullptr;
		int currentModuleNumber = 0;
		WindowManager windowManager;

	public:
		LilEddieGame();
		~LilEddieGame();

		//Both methods returns 0 on success, error code on fail (see defs)
		int CompileGameDLL();
		int ReloadGameDLL();

	protected:
		void Init() override;
		void Start() override;
		void Update() override;
	};
}
