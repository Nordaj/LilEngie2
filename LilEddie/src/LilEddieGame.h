#pragma once

#include "WindowManager.h"
#include <LilEngie.h>

namespace LilEddie
{
	class IEditorWindow;

	class LilEddieGame : public Game
	{
	private:
		WindowManager windowManager;

	public:
		LilEddieGame();
		~LilEddieGame();

	protected:
		void Init() override;
		void Start() override;
		void Update() override;
	};
}
