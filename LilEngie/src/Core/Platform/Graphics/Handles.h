#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	class LIL_API IShader
	{
	public:
		virtual ~IShader() {}
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
