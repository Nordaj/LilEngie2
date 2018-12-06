#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	class PoolAllocator
	{
	private:
		void* data;
		void* firstFree;
		unsigned int size;
		unsigned int nodeSize;

	public:
		//Size is measured in bytes
		PoolAllocator(unsigned int size, unsigned int nodeSize);
		~PoolAllocator();

		//Size is measured in bytes
		void* Alloc();
		void Free(void* location);
	};
}
