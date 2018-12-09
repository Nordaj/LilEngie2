#include <chrono>
#include <string>
#include "Log.h"
#include <Core/Game/ServiceLocator.h>
#include "DebugTimer.h"

using namespace std::chrono;
typedef high_resolution_clock::time_point TimePoint;
typedef high_resolution_clock Clock;

namespace LilEngie
{
	DebugTimer::DebugTimer()
	{
		start = Clock::now();
	}

	double DebugTimer::GetElapsed(TimeUnit unit)
	{
		switch (unit)
		{
			case LilEngie::ns:
				return duration_cast<nanoseconds>(Clock::now() - start).count();
			case LilEngie::us:
				return duration_cast<microseconds>(Clock::now() - start).count();
			case LilEngie::ms:
				return duration_cast<milliseconds>(Clock::now() - start).count();
			case LilEngie::s:
				return duration_cast<seconds>(Clock::now() - start).count();
			case LilEngie::m:
				return duration_cast<minutes>(Clock::now() - start).count();
			case LilEngie::h:
				return duration_cast<hours>(Clock::now() - start).count();
			default:
				return duration_cast<milliseconds>(Clock::now() - start).count();
		}
	}

	void DebugTimer::LogElapsed(TimeUnit unit)
	{
		std::string u;
		switch (unit)
		{
			case LilEngie::Nanoseconds:
				u = "ns";
				break;
			case LilEngie::Microseconds:
				u = "us";
				break;
			case LilEngie::Milliseconds:
				u = "ms";
				break;
			case LilEngie::Seconds:
				u = "s";
				break;
			case LilEngie::Minutes:
				u = "m";
				break;
			case LilEngie::Hours:
				u = "h";
				break;
			default:
				break;
		}

		if (ServiceLocator::Log() != nullptr)
			ServiceLocator::Log()->Print(GetElapsed(unit), u, " elapsed");
	}
}
