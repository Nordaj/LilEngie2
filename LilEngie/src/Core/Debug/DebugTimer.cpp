#include <chrono>
#include <string>
#include "Log.h"
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
		duration<double> d = Clock::now() - start;
		switch (unit)
		{
			case LilEngie::ns:
				return duration_cast<nanoseconds>(d).count();
			case LilEngie::us:
				return duration_cast<nanoseconds>(d).count() * 1e-3;
			case LilEngie::ms:
				return duration_cast<nanoseconds>(d).count() * 1e-6;
			case LilEngie::s:
				return duration_cast<nanoseconds>(d).count() * 1e-9;
			case LilEngie::m:
				return duration_cast<seconds>(d).count() / 60.f;
			case LilEngie::h:
				return duration_cast<minutes>(d).count() / 60.f;
			default:
				return duration_cast<milliseconds>(d).count();
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

		LIL_LOG(GetElapsed(unit), u, " elapsed");
	}
}
