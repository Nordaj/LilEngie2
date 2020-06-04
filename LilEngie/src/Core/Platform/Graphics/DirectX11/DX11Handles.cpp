#include <d3d11.h>
#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>
#include "DX11Handles.h"

namespace LilEngie
{
	DX11Shader::~DX11Shader()
	{
		if (vertShader != nullptr)
		{
			vertShader->Release();
			vertShader = nullptr;
		}

		if (fragShader != nullptr)
		{
			fragShader->Release();
			fragShader = nullptr;
		}
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

	DX11CBuffer::~DX11CBuffer()
	{
		if (buffer != nullptr)
		{
			buffer->Release();
			buffer = nullptr;
		}

		if (data != nullptr)
		{
			delete[] data;
			data = nullptr;
		}
	}

	DX11Texture::~DX11Texture()
	{
		if (texture)
		{
			texture->Release();
			texture = nullptr;
		}

		if (view)
		{
			view->Release();
			view = nullptr;
		}

		if (samplerState)
		{
			samplerState->Release();
			samplerState = nullptr;
		}
	}
}
