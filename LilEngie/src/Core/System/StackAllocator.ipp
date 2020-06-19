#pragma once

namespace LilEngie
{
	template<typename T>
	T* StackAllocator::Alloc(unsigned int size)
	{
		T* theirs = end;
		end = (char*)end + (sizeof(T) * size);
		return theirs;
	}
}
