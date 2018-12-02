#include <iostream>
#include "Log.h"

namespace LilEngie
{
	Log::Log()
	{
		//Only want to log fatal errors in release
	#ifdef LIL_RELEASE
		verbosity = Verbosity::Fatal;
	#endif // LIL_RELEASE
	}

	std::string Log::VerbosityToString(Verbosity v)
	{
		switch (v)
		{
			case Verbosity::Verbose:
				return "Verbose";
				break;
			case Verbosity::Log:
				return "Log";
				break;
			case Verbosity::Warning:
				return "Warning";
				break;
			case Verbosity::Error:
				return "Error";
				break;
			case Verbosity::Fatal:
				return "Fatal";
				break;
			default:
				break;
		}
	}
}
