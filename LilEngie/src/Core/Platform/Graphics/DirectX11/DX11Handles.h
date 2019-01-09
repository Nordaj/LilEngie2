#pragma once

#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>
#include <Core/Core.h>

class ID3D11InputLayout;
class ID3D11VertexShader;
class ID3D11PixelShader;
class D3D11Buffer;

namespace LilEngie
{
	class DX11Shader : public IShader
	{
		friend class DX11Graphics;

	private:
		ID3D11VertexShader* vertShader;
		ID3D11PixelShader* fragShader;

	public:
		~DX11Shader();
	};

	class DX11InputLayout : public IInputLayout
	{
		friend class DX11Graphics;

	private:
		ID3D11InputLayout* layout;

	public:
		~DX11InputLayout();
	};

	class DX11VertexBuffer : public IVertexBuffer
	{
		friend class DX11Graphics;

	private:
		ID3D11Buffer* buffer;

	public:
		~DX11VertexBuffer();
	};

	class DX11IndexBuffer : public IIndexBuffer
	{
		friend class DX11Graphics;

	private:
		ID3D11Buffer* buffer;

	public:
		~DX11IndexBuffer();
	};
}
