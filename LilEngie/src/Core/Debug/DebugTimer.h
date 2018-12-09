#pragma once

#include <chrono>
#include <Core/Core.h>

namespace LilEngie
{
	enum TimeUnit
	{
		Nanoseconds = 0,
		Microseconds = 1,
		Milliseconds = 2,
		Seconds = 3,
		Minutes = 4,
		Hours = 5,

		ns = 0,
		us = 1,
		ms = 2,
		s = 3,
		m = 4,
		h = 5,
	};

	class LIL_API DebugTimer
	{
	private:
		std::chrono::high_resolution_clock::time_point start;

	public:
		DebugTimer();

		double GetElapsed(TimeUnit unit = TimeUnit::ms);
		void LogElapsed(TimeUnit unit = TimeUnit::ms);
	};
}
