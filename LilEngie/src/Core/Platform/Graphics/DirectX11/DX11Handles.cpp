#include <d3d11.h>
#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>
#include "DX11Handles.h"

namespace LilEngie
{
	DX11VertexShader::~DX11VertexShader()
	{
		if (shader == nullptr)
			return;

		shader->Release();
		shader = nullptr;
	}

	DX11FragmentShader::~DX11FragmentShader()
	{
		if (shader == nullptr)
			return;

		shader->Release();
		shader = nullptr;
	}

	DX11InputLayout::~DX11InputLayout()
	{
		if (layout == nullptr)
			return;

		layout->Release();
		layout = nullptr;
	}

	DX11VertexBuffer::~DX11VertexBuffer()
	{
		if (buffer == nullptr)
			return;

		buffer->Release();
		buffer = nullptr;
	}

	DX11IndexBuffer::~DX11IndexBuffer()
	{
		if (buffer == nullptr)
			return;

		buffer->Release();
		buffer = nullptr;
	}
}
