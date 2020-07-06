#pragma once

#include <LilEngie.h>

namespace LilEddie
{
	class IEditorWindow;

	class LilEddieGame : public Game
	{
	private:
		std::vector<IEditorWindow*> windows;

	public:
		LilEddieGame();
		~LilEddieGame();

	protected:
		void Init() override;
		void Start() override;
		void Update() override;
	};
}
