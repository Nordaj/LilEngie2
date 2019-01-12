#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <Windows.h>
#include <Core/Debug/Log.h>
#include <Core/Game/ServiceLocator.h>
#include <Core/Platform/Window/Window.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>
#include "DX11Handles.h"
#include "DX11Graphics.h"

#define GFX_ERROR(x) { ServiceLocator::Log()->Print(Verbosity::Error, x); return; }

namespace LilEngie
{
	static HRESULT CompileShader(LPCWSTR file, ID3DBlob** blob, LPCSTR profile);

	struct D3D11Ctx
	{
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		ID3D11RenderTargetView* renderTargetView;
		IDXGISwapChain* swapChain;
	};

	DX11Graphics::~DX11Graphics()
	{
		if (ctx != nullptr)
		{
			ServiceLocator::Log()->Print(Verbosity::Warning, "Direct3D11 context destroyed incorrectly. Please use IGraphics::ReleaseGraphicsContext().");
			Shutdown();
		}
	}

	void DX11Graphics::Init(const WinProp &windowProperties)
	{
		HRESULT hr = S_OK;

		ctx = new D3D11Ctx();

		//Setup device and swap chain
		DXGI_SWAP_CHAIN_DESC scd = {};
		scd.BufferCount = 1;
		scd.BufferDesc.Width = windowProperties.width;
		scd.BufferDesc.Height = windowProperties.height;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = (HWND)windowProperties.hwnd;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.Windowed = true;

		//Might want to change later
		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };

		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			NULL,
			featureLevels,
			3,
			D3D11_SDK_VERSION,
			&scd,
			&(ctx->swapChain),
			&(ctx->device),
			NULL,
			&(ctx->deviceContext)
			);
		if (FAILED(hr))
			GFX_ERROR("Failed to create Direct3D11 device and swap chain.");

		//Setup render target view
		SetupRenderTargetView();

		//Set viewport
		D3D11_VIEWPORT viewport = {};
		viewport.Width = (float)windowProperties.width;
		viewport.Height = (float)windowProperties.width;
		viewport.MaxDepth = 1;
		ctx->deviceContext->RSSetViewports(1, &viewport);
	}

	void DX11Graphics::SetClearColor(float r, float g, float b, float a)
	{
		clearColor[0] = r;
		clearColor[1] = g;
		clearColor[2] = b;
		clearColor[3] = a;
	}

	void DX11Graphics::Clear()
	{
		ctx->deviceContext->ClearRenderTargetView(ctx->renderTargetView, clearColor);
	}

	void DX11Graphics::Render()
	{
		HRESULT hr = S_OK;
		hr = ctx->swapChain->Present(0, 0);
		if (FAILED(hr))
			GFX_ERROR("Failed to present Direct3D11 swap chain.");
	}

	void DX11Graphics::Resize(int width, int height)
	{
		//Need to release render target view before resizing
		ctx->renderTargetView->Release();
		ctx->renderTargetView = nullptr;

		//Resize the buffers
		ctx->swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		//Setup new render target view
		SetupRenderTargetView();

		//Setup viewport
		D3D11_VIEWPORT viewport = {};
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.MaxDepth = 1;
		ctx->deviceContext->RSSetViewports(1, &viewport);
	}

	IShader* DX11Graphics::CreateShader(const std::string &vert, const std::string &frag)
	{
		return CreateShader(vert, frag, nullptr, nullptr, 0);
	}

	IShader* DX11Graphics::CreateShader(const std::string &vert, const std::string &frag, IInputLayout** layout, InputElement* elements, uint numElements)
	{
		HRESULT hr = S_OK;
		DX11Shader* shader = new DX11Shader();

		//Vertex
		{
			//Add extension if not yet added
			std::string vFile;
			if (vert.substr(vert.size() - 5, 5) != ".hlsl")
				vFile = vert + ".hlsl";
			else
				vFile = vert;

			//Convert string to wstring
			size_t converted;
			wchar_t* wFile = new wchar_t[vFile.size() + 1];
			mbstowcs_s(&converted, wFile, vFile.size() + 1, vFile.c_str(), vFile.size());

			//Compile shader into shader blob
			ID3DBlob* shaderBlob;
			hr = CompileShader(wFile, &shaderBlob, "vs_5_0");
			delete wFile;
			if (hr != S_OK)
				return nullptr;

			//Create input layout
			if (layout != nullptr)
				*layout = CreateLayout(shaderBlob, elements, numElements);

			//Create shader
			hr = ctx->device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader->vertShader);
			if (hr != S_OK)
			{
				LIL(Log)->Print(Verbosity::Error, "Could not create vertex shader.");
				shaderBlob->Release();
				return nullptr;
			}
			shaderBlob->Release();
		}

		//Fragment
		{
			//Add extension if not yet added
			std::string fFile;
			if (frag.substr(frag.size() - 5, 5) != ".hlsl")
				fFile = frag + ".hlsl";
			else
				fFile = frag;

			//Convert string to wstring
			size_t converted;
			wchar_t* wFile = new wchar_t[fFile.size() + 1];
			mbstowcs_s(&converted, wFile, fFile.size() + 1, fFile.c_str(), fFile.size());

			//Compile shader into shader blob
			ID3DBlob* shaderBlob;
			hr = CompileShader(wFile, &shaderBlob, "ps_5_0");
			delete wFile;
			if (hr != S_OK)
				return nullptr;

			//Create shader
			hr = ctx->device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &shader->fragShader);
			if (hr != S_OK)
			{
				LIL(Log)->Print(Verbosity::Error, "Could not create fragment shader.");
				shaderBlob->Release();
				return nullptr;
			}
			shaderBlob->Release();
		}

		return shader;
	}

	void DX11Graphics::SetShader(IShader* shader)
	{
		ctx->deviceContext->VSSetShader(((DX11Shader*)shader)->vertShader, NULL, 0);
		ctx->deviceContext->PSSetShader(((DX11Shader*)shader)->fragShader, NULL, 0);
	}

	void DX11Graphics::ReleaseShader(IShader** shader)
	{
		((DX11Shader*)(*shader))->vertShader->Release();
		((DX11Shader*)(*shader))->vertShader = nullptr;

		((DX11Shader*)(*shader))->fragShader->Release();
		((DX11Shader*)(*shader))->fragShader = nullptr;

		delete *shader;
		*shader = nullptr;
	}

	void DX11Graphics::SetInputLayout(IInputLayout* layout)
	{
		ctx->deviceContext->IASetInputLayout(((DX11InputLayout*)layout)->layout);
	}

	void DX11Graphics::ReleaseInputLayout(IInputLayout** layout)
	{
		((DX11InputLayout*)(*layout))->layout->Release();
		((DX11InputLayout*)(*layout))->layout = nullptr;
		delete *layout;
		*layout = nullptr;
	}

	IVertexBuffer* DX11Graphics::CreateVertexBuffer(float* verts, uint size)
	{
		HRESULT hr = S_OK;

		//Create buffer structs
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = size;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = verts;
		sd.SysMemPitch = NULL;
		sd.SysMemSlicePitch = NULL;

		//Create and return buffer
		DX11VertexBuffer* buffer = new DX11VertexBuffer();
		hr = ctx->device->CreateBuffer(&bd, &sd, &buffer->buffer);
		if (FAILED(hr))
		{
			LIL(Log)->Print(Verbosity::Error, "Could not create vertex buffer.");
			return nullptr;
		}

		return buffer;
	}

	void DX11Graphics::BindVertexBuffer(IVertexBuffer* vBuffer, uint stride)
	{
		uint offset = 0;
		ctx->deviceContext->IASetVertexBuffers(0, 1, &((DX11VertexBuffer*)vBuffer)->buffer, &stride, &offset);
	}

	void DX11Graphics::ReleaseVertexBuffer(IVertexBuffer** vBuffer)
	{
		((DX11VertexBuffer*)(*vBuffer))->buffer->Release();
		((DX11VertexBuffer*)(*vBuffer))->buffer = nullptr;
		delete *vBuffer;
		*vBuffer = nullptr;
	}

	IIndexBuffer* DX11Graphics::CreateIndexBuffer(uint* inds, uint size)
	{
		HRESULT hr = S_OK;

		//Create buffer structs
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = size;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = inds;
		sd.SysMemPitch = NULL;
		sd.SysMemSlicePitch = NULL;

		//Create and return buffer
		DX11IndexBuffer* buffer = new DX11IndexBuffer();
		hr = ctx->device->CreateBuffer(&bd, &sd, &buffer->buffer);
		if (FAILED(hr))
		{
			LIL(Log)->Print(Verbosity::Error, "Could not create index buffer.");
			return nullptr;
		}

		return buffer;
	}

	void DX11Graphics::BindIndexBuffer(IIndexBuffer* iBuffer)
	{
		ctx->deviceContext->IASetIndexBuffer(((DX11IndexBuffer*)iBuffer)->buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11Graphics::ReleaseIndexBuffer(IIndexBuffer** iBuffer)
	{
		((DX11IndexBuffer*)(*iBuffer))->buffer->Release();
		((DX11IndexBuffer*)(*iBuffer))->buffer = nullptr;
		delete *iBuffer;
		*iBuffer = nullptr;
	}

	void DX11Graphics::Draw(uint indexCount)
	{
		ctx->deviceContext->DrawIndexed(indexCount, 0, 0);
	}

	void DX11Graphics::Shutdown()
	{
		ctx->device->Release();
		ctx->deviceContext->Release();
		ctx->renderTargetView->Release();
		ctx->swapChain->Release();
		delete ctx;
		ctx = nullptr;
	}

	static HRESULT CompileShader(LPCWSTR file, ID3DBlob** blob, LPCSTR profile)
	{
		HRESULT hr = S_OK;

		//Compilation flags
		unsigned int flags = D3DCOMPILE_ENABLE_STRICTNESS;
		#ifdef LIL_DEBUG
		flags |= D3DCOMPILE_DEBUG;
		#endif //LIL_DEBUG

		//Compile vertex shader
		ID3DBlob *shaderBlob = nullptr;
		ID3DBlob *errorBlob = nullptr;
		hr = D3DCompileFromFile(
			file, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main", profile, flags, NULL, &shaderBlob, &errorBlob);

		//If failed to compile vertex shader
		if (FAILED(hr))
		{
			//If i have an error to give
			if (errorBlob)
			{
				LIL(Log)->Print(Verbosity::Error, "Shader compilation error: \n", (char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			else
				LIL(Log)->Print(Verbosity::Error, "Shader compilation failed. Could not recieve error message. Check the shader directory.");

			//If i have a shader blob
			if (shaderBlob)
				shaderBlob->Release();

			return hr;
		}

		//Set blob
		*blob = shaderBlob;

		return hr;
	}

	IInputLayout* DX11Graphics::CreateLayout(void* shdBlb, InputElement* elements, uint numElements)
	{
		HRESULT hr = S_OK;

		//Translate layout lambda
		auto TranslateFormat = [](InputFormat f) -> DXGI_FORMAT
		{
			switch (f)
			{
				case LilEngie::FLOAT_R32G32B32A32:
					return DXGI_FORMAT_R32G32B32A32_FLOAT;
					break;
				case LilEngie::FLOAT_R32G32B32:
					return DXGI_FORMAT_R32G32B32_FLOAT;
					break;
				case LilEngie::FLOAT_R32G32:
					return DXGI_FORMAT_R32G32_FLOAT;
					break;
				case LilEngie::FLOAT_R32:
					return DXGI_FORMAT_R32_FLOAT;
					break;
				case LilEngie::UINT_R32:
					return DXGI_FORMAT_R32_UINT;
					break;
				default:
					return DXGI_FORMAT_UNKNOWN;
					break;
			}
		};

		//Create input layout array
		D3D11_INPUT_ELEMENT_DESC* e = new D3D11_INPUT_ELEMENT_DESC[numElements];
		for (int i = 0; i < numElements; i++)
		{
			e[i].SemanticName = elements[i].name.c_str();
			e[i].SemanticIndex = 0;
			e[i].Format = TranslateFormat(elements[i].format);
			e[i].InputSlot = 0;
			e[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			e[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			e[i].InstanceDataStepRate = 0;
		}

		//Create the input layout
		DX11InputLayout* layout = new DX11InputLayout();
		hr = ctx->device->CreateInputLayout(e, numElements, ((ID3DBlob*)shdBlb)->GetBufferPointer(), ((ID3DBlob*)shdBlb)->GetBufferSize(), &layout->layout);
		if (hr != S_OK)
		{
			LIL(Log)->Print(Verbosity::Error, "Could not create input layout.");
			delete[] e;
			return nullptr;
		}

		//Clean and return
		delete[] e;
		return layout;
	}

	void DX11Graphics::SetupRenderTargetView()
	{
		HRESULT hr = S_OK;

		//Release if necessary
		if (ctx->renderTargetView != nullptr)
			ctx->renderTargetView->Release();

		//Setup render target view
		ID3D11Texture2D* tex;
		hr = ctx->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tex);
		if (FAILED(hr))
			GFX_ERROR("Failed to get Direct3D11 swap chain back buffer.");

		hr = ctx->device->CreateRenderTargetView(tex, NULL, &ctx->renderTargetView);
		if (FAILED(hr))
			GFX_ERROR("Failed to create Direct3D11 render target view.");

		ctx->deviceContext->OMSetRenderTargets(1, &ctx->renderTargetView, NULL);

		tex->Release();
	}
}
