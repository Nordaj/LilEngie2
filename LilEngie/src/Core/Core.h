#pragma once

#include "Common.h"

#if defined(LIL_WINDOWS)
	#if defined(LIL_BUILD_DLL)
		//BUILDING AS DYNAMIC LIB
		#define LIL_API __declspec(dllexport)
	#elif defined(LIL_DLL)
		//CALLING AS DYNAMIC LIB
		#define LIL_API __declspec(dllimport)
	#else
		//FOR STATIC LIB
		#define LIL_API
	#endif
#endif //LIL_WINDOWS
