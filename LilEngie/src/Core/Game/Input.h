#pragma once

#include <Core/Core.h>
#include <Core/Platform/Window/Keys.h>

namespace LilEngie
{
	class Game;

	class LIL_API Input
	{
	public:
		static Input* core;
		
		bool* currentKeys;
		bool* prevKeys;

	private:
		Game* game;

	public:
		Input();
		~Input();

		void Init(Game* game);

		bool GetKey(Key key);
		bool GetKeyDown(Key key);
		bool GetKeyUp(Key key);

		void CacheKeyStates();
	};
}
