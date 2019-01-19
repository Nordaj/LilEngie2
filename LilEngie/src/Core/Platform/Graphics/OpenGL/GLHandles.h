#pragma once

#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>
#include <Core/Common.h>

namespace LilEngie
{
	class GLShader : public IShader
	{
		friend class GLGraphics;

	private:
		uint shader;

	public:
		~GLShader();
	};

	class GLInputLayout : public IInputLayout
	{
		friend class GLGraphics;

	private:
		InputElement* elements;
		uint size;

	public:
		~GLInputLayout();
	};

	class GLVertexBuffer : public IVertexBuffer
	{
		friend class GLGraphics;

	private:
		uint buffer;

	public:
		~GLVertexBuffer();
	};

	class GLIndexBuffer : public IIndexBuffer
	{
		friend class GLGraphics;

	private:
		uint buffer;

	public:
		~GLIndexBuffer();
	};

	class GLCBuffer : public ICBuffer
	{
		friend class GLGraphics;

	private:
		uint buffer;
		void* data;
		uint size;

	public:
		~GLCBuffer();
	};
}
