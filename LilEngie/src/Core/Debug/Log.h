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
		Verbosity verbosity = Verbosity::Log;

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

#include "Log.ipp"
