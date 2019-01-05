#include <GL/glew.h>
#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>
#include "GLHandles.h"

namespace LilEngie
{
	GLVertexShader::~GLVertexShader()
	{
		if (shader == 0)
			return;

		glDeleteShader(shader);
		shader = 0;
	}

	GLFragmentShader::~GLFragmentShader()
	{
		if (shader == 0)
			return;

		glDeleteShader(shader);
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
}
