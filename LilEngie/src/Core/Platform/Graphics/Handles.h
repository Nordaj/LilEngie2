#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	class LIL_API IVertexShader
	{
	public:
		virtual ~IVertexShader() {}
	};

	class LIL_API IFragmentShader
	{
	public:
		virtual ~IFragmentShader() {}
	};

	class LIL_API IInputLayout
	{
	public:
		virtual ~IInputLayout() {}
	};

	class LIL_API IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() {}
	};

	class LIL_API IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() {}
	};
}
