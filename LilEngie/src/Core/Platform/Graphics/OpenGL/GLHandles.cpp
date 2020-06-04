#include <GL/glew.h>
#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>
#include "GLHandles.h"

namespace LilEngie
{
	GLShader::~GLShader()
	{
		if (shader == 0)
			return;

		glDeleteProgram(shader);
		shader = 0;
	}

	GLInputLayout::~GLInputLayout()
	{ 
		//Handle is not responsible for managing the array.
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		if (buffer == 0)
			return;

		glDeleteBuffers(1, &buffer);
		buffer = 0;
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		if (buffer == 0)
			return;

		glDeleteBuffers(1, &buffer);
		buffer = 0;
	}

	GLCBuffer::~GLCBuffer()
	{
		if (buffer != 0)
		{
			glDeleteBuffers(1, &buffer);
			buffer = 0;
		}

		if (data != nullptr)
		{
			delete[] data;
			data = nullptr;
		}
	}

	GLTexture::~GLTexture()
	{
		if (texture == 0)
			return;

		glDeleteTextures(1, &texture);
		texture = 0;
	}
}
