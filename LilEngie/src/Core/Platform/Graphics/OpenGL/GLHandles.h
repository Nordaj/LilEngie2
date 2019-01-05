#pragma once

#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>
#include <Core/Common.h>

namespace LilEngie
{
	class GLVertexShader : public IVertexShader
	{
		friend class GLGraphics;

	private:
		uint shader;

	public:
		GLVertexShader() {}
		~GLVertexShader();
	};

	class GLFragmentShader : public IFragmentShader
	{
		friend class GLGraphics;

	private:
		uint shader;

	public:
		GLFragmentShader() {}
		~GLFragmentShader();
	};

	class GLInputLayout : public IInputLayout
	{
		friend class GLGraphics;

	private:
		InputElement* elements;
		uint size;

	public:
		GLInputLayout() {}
		~GLInputLayout();
	};

	class GLVertexBuffer : public IVertexBuffer
	{
		friend class GLGraphics;

	private:
		uint buffer;

	public:
		GLVertexBuffer() {}
		~GLVertexBuffer();
	};

	class GLIndexBuffer : public IIndexBuffer
	{
		friend class GLGraphics;

	private:
		uint buffer;

	public:
		GLIndexBuffer() {}
		~GLIndexBuffer();
	};
}
