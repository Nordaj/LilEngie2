#pragma once

#include <functional>
#include <Core/Core.h>

namespace LilEngie
{
	class LIL_API Game
	{
	private:
		bool isRunning = true;

	public:
		Game(std::function<void()> start, std::function<void()> update);
		~Game();
	};
}
