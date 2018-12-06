#include <memory>
#include <Core/Debug/Log.h>
#include <Core/Game/ServiceLocator.h>
#include "PoolAllocator.h"

namespace LilEngie
{
	PoolAllocator::PoolAllocator(unsigned int size, unsigned int nodeSize)
		: size(size), nodeSize(nodeSize) 
	{
		if (size % nodeSize != 0)
			LIL(Log)->Print(Verbosity::Error, "PoolAllocator size must be a mutliple of nodeSize.");
		if (nodeSize < sizeof(void*))
			LIL(Log)->Print(Verbosity::Error, "PoolAllocator nodeSize must be larger than a pointer.");

		data = new char[size]();
		firstFree = data;

		//Setup linked list
		char* d = (char*)data;
		for (char* i = d; i < d + size; i += nodeSize)
			*(char**)i = i + nodeSize;
	}

	PoolAllocator::~PoolAllocator()
	{
		delete data;
	}

	void* PoolAllocator::Alloc()
	{
		void* theirs = firstFree;
		firstFree = *(void**)firstFree;
		memset(theirs, 0, nodeSize);
		return theirs;
	}

	void PoolAllocator::Free(void* location)
	{
		*(void**)location = firstFree;
		firstFree = location;
	}
}
