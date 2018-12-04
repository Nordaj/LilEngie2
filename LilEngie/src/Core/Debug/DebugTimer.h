#pragma once

#include <chrono>
#include <Core/Core.h>

namespace LilEngie
{
	class LIL_API DebugTimer
	{
	private:
		std::chrono::high_resolution_clock::time_point start;

	public:
		DebugTimer();

		double GetElapsed();
		void LogElapsed();
	};
}
