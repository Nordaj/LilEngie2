#include <memory>
#include "StackAllocator.h"

namespace LilEngie
{
	StackAllocator::StackAllocator(unsigned int size)
	{
		data = new char[size];
		this->size = size;
	}

	StackAllocator::~StackAllocator()
	{
		delete data;
	}

	void* StackAllocator::Alloc(unsigned int size)
	{
		void* theirs = end;
		end = (char*)end + size;
		return theirs;
	}

	void StackAllocator::Free(void* location)
	{
		//0 out the memory
		memset(location, 0, (char*)end - location);
		end = location;
	}

	void StackAllocator::FreeAll()
	{
		delete data;
		data = nullptr;
		end = nullptr;
		size = 0;
	}
}
