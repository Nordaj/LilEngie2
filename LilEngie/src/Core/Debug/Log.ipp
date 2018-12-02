#pragma once

#include <iostream>

namespace LilEngie
{
	template<typename T, typename... Args>
	void Log::Print(Verbosity verbosity, T first, Args... args)
	{
		if (verbosity >= this->verbosity)
		{
			std::cout << "[" << VerbosityToString(verbosity) << "] ";
			PrintInternal(first, args...);
		}
	}

	template<typename T, typename... Args>
	void Log::Print(T first, Args... args)
	{
		Print(Verbosity::Log, first, args...);
	}

	template <typename T, typename... Args>
	void Log::PrintGroup(Verbosity verbosity, T first, Args... args)
	{
		if (verbosity >= this->verbosity)
		{
			std::cout << "[" << VerbosityToString(verbosity) << "] ";
			std::cout << "(";
			PrintGroupNoParen(first, args...);
			std::cout << ")" << std::endl;
		}
	}

	template <typename T, typename... Args>
	void Log::PrintGroup(T first, Args... args)
	{
		PrintGroup(Verbosity::Log, first, args...);
	}

	template<typename T, typename... Args>
	void Log::PrintInternal(T first, Args... args)
	{
		std::cout << first;
		PrintInternal(args...);
	}

	template<typename T>
	void Log::PrintInternal(T var)
	{
		std::cout << var << std::endl;
	}

	template<typename T>
	void Log::PrintGroupNoParen(T var)
	{
		std::cout << var;
	}

	template<typename T, typename... Args>
	void Log::PrintGroupNoParen(T first, Args... args)
	{
		std::cout << first << ", ";
		PrintGroupNoParen(args...);
	}
}
