#pragma once

#include <string>
#include <vector>
#include <Core/Core.h>

namespace LilEngie
{
	//This should be enough for now. probably should add more later
	enum InputFormat
	{
		FLOAT_R32G32B32A32,
		FLOAT_R32G32B32,
		FLOAT_R32G32,
		FLOAT_R32,
		UINT_R32
	};

	class LIL_API InputElement
	{
	public:
		InputElement() {}
		InputElement(const std::string &name, InputFormat format, unsigned int offset)
			: name(name), format(format), offset(offset) { }

		std::string name;
		InputFormat format;
		unsigned int offset;
	};

	class LIL_API InputLayoutData
	{
	public:
		std::vector<InputElement> elements;
	};
}
