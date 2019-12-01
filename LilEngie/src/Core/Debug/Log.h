#pragma once

#include <string>
#include <Core/Core.h>

namespace LilEngie
{
	enum class Verbosity
	{
		Verbose,
		Log,
		Warning,
		Error,
		Fatal
	};

	class LIL_API Log
	{
	public:
		static Log* core;

		Verbosity verbosity;

	public:
		Log();

		template<typename T, typename... Args>
		void Print(Verbosity verbosity, T first, Args... args);

		template<typename T, typename... Args>
		void Print(T first, Args... args);

		template<typename T, typename... Args>
		void PrintGroup(Verbosity verbosity, T first, Args... args);

		template<typename T, typename... Args>
		void PrintGroup(T first, Args... args);

	private:
		template<typename T, typename... Args>
		void PrintInternal(T first, Args... args);

		template<typename T>
		void PrintInternal(T var);

		template<typename T>
		void PrintGroupNoParen(T var);

		template<typename T, typename... Args>
		void PrintGroupNoParen(T first, Args... args);

		std::string VerbosityToString(Verbosity v);
	};
}

#define LIL_LOG(...) if (Log::core) Log::core->Print(__VA_ARGS__);
#define LIL_WARN(...) if (Log::core) Log::core->Print(Verbosity::Warning, __VA_ARGS__);
#define LIL_ERROR(...) if (Log::core) Log::core->Print(Verbosity::Error, __VA_ARGS__);
#define LIL_FATAL(...) if (Log::core) Log::core->Print(Verbosity::Fatal, __VA_ARGS__);

#include "Log.ipp"
