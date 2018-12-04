#include <chrono>
#include "Log.h"
#include <Core/Game/ServiceLocator.h>
#include "DebugTimer.h"

using namespace std::chrono;
typedef high_resolution_clock::time_point TimePoint;
typedef high_resolution_clock Clock;
typedef milliseconds ms;

namespace LilEngie
{
	DebugTimer::DebugTimer()
	{
		start = Clock::now();
	}

	double DebugTimer::GetElapsed()
	{
		return duration_cast<ms>(Clock::now() - start).count();
	}

	void DebugTimer::LogElapsed()
	{
		if (ServiceLocator::Log() != nullptr)
			ServiceLocator::Log()->Print(duration_cast<ms>(Clock::now() - start).count(), "ms elapsed");
	}
}
