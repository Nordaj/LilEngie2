#pragma once

#include <memory>
#include <Core/Core.h>

namespace LilEngie
{
	class LIL_API StackAllocator
	{
	private:
		void* data;
		unsigned int size;
		void* end;

	public:
		StackAllocator(unsigned int size);
		~StackAllocator();

		//Size is measured in bytes
		void* Alloc(unsigned int size);
		void Free(void* location);
		void FreeAll();

		//Size is amount of T (not in bytes).
		template<typename T>
		T* Alloc(unsigned int size = 1);
	};
}

#include "StackAllocator.ipp"
