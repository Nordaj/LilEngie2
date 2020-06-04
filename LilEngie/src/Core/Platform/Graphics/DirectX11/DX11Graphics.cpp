#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <Windows.h>
#include <Core/Debug/Log.h>
#include <Core/Platform/Window/Window.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>
#include "DX11Handles.h"
#include "DX11Graphics.h"

#define GFX_ERROR(x) { LIL_ERROR(x); return; }

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
			LIL_WARN("Direct3D11 context destroyed incorrectly. Please use IGraphics::ReleaseGraphicsContext().");
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
		viewport.Height = (float)windowProperties.height;
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
		hr = ctx->swapChain->Present(1, 0); //1=vsync
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
				LIL_ERROR("Could not create vertex shader.");
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
				LIL_ERROR("Could not create fragment shader.");
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
			LIL_ERROR("Could not create vertex buffer.");
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
			LIL_ERROR("Could not create index buffer.");
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

	//Currently inilializes the buffer with useless memory for no reason. fix it
	ICBuffer* DX11Graphics::CreateCBuffer(uint size, void* initData)
	{
		HRESULT hr = S_OK;

		//DX11 requires size in multiples of 16, allocate extra if needed
		size = 16 * ((size - 1) / 16 + 1);

		//Setup cbuffer
		DX11CBuffer* cBuffer = new DX11CBuffer();
		cBuffer->data = new char[size];
		cBuffer->size = size;
		if (initData != nullptr)
			memcpy(cBuffer->data, initData, size);

		//Create buffer
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = size;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = NULL;
		bd.StructureByteStride = NULL;

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = cBuffer->data;
		sd.SysMemPitch = 0;
		sd.SysMemSlicePitch = 0;

		hr = ctx->device->CreateBuffer(&bd, &sd, &cBuffer->buffer);
		if (FAILED(hr))
		{
			LIL_ERROR("Could not create constant buffer.");
			return nullptr;
		}

		return cBuffer;
	}

	void* DX11Graphics::GetCBufferPtr(ICBuffer* cBuffer)
	{
		return ((DX11CBuffer*)cBuffer)->data;
	}

	uint DX11Graphics::GetCBufferSize(ICBuffer* cBuffer)
	{
		return ((DX11CBuffer*)cBuffer)->size;
	}

	void DX11Graphics::UpdateCBuffer(ICBuffer* cBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE msr = {};
		ctx->deviceContext->Map(((DX11CBuffer*)cBuffer)->buffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &msr);
		memcpy(msr.pData, ((DX11CBuffer*)cBuffer)->data, ((DX11CBuffer*)cBuffer)->size);
		ctx->deviceContext->Unmap(((DX11CBuffer*)cBuffer)->buffer, 0);
	}

	void DX11Graphics::BindCBuffer(ICBuffer* cBuffer, ShaderType type, uint slot)
	{
		switch (type)
		{
			case ShaderType::Fragment:
				ctx->deviceContext->PSSetConstantBuffers(slot, 1, &((DX11CBuffer*)cBuffer)->buffer);
				break;
			case ShaderType::Vertex:
				ctx->deviceContext->VSSetConstantBuffers(slot, 1, &((DX11CBuffer*)cBuffer)->buffer);
				break;
			default:
				break;
		}
	}

	void DX11Graphics::ReleaseCBuffer(ICBuffer** cBuffer)
	{
		delete[] ((DX11CBuffer*)*cBuffer)->data;
		((DX11CBuffer*)*cBuffer)->data = nullptr;

		((DX11CBuffer*)*cBuffer)->buffer->Release();
		((DX11CBuffer*)*cBuffer)->buffer = nullptr;

		delete *cBuffer;
		*cBuffer = nullptr;
	}

	ITexture* DX11Graphics::CreateTexture(uint w, uint h, TextureFormat format, void* data, bool wrap, bool mipmaps, bool filter)
	{
		HRESULT hr = S_OK;

		//Create texture obj
		DX11Texture* tex = new DX11Texture();

		//Get format
		DXGI_FORMAT dxFormat;
		uint pSize;
		switch (format)
		{
			case LilEngie::TextureFormat::R8:
				dxFormat = DXGI_FORMAT_R8_UNORM;
				pSize = 1;
				break;
			case LilEngie::TextureFormat::R8G8:
				dxFormat = DXGI_FORMAT_R8G8_UNORM;
				pSize = 2;
				break;
			case LilEngie::TextureFormat::R8G8B8A8:
				dxFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
				pSize = 4;
				break;
			default:
				dxFormat = DXGI_FORMAT_R8_UNORM;
				pSize = 1;
				break;
		}

		//Texture description
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = w;
		desc.Height = h;
		desc.MipLevels = 1; //docs says use 0 for auto generated  miplevels but that doesnt work???
		desc.ArraySize = 1;
		desc.Format = dxFormat;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = mipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		//Initial data
		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = data;
		initData.SysMemPitch = w * pSize;

		//Create the texture
		hr = ctx->device->CreateTexture2D(&desc, &initData, &tex->texture);
		if (FAILED(hr))
		{
			LIL_ERROR("Could not create Texture2D");
			delete tex;
			return nullptr;
		}

		//Create resource view
		hr = ctx->device->CreateShaderResourceView(tex->texture, nullptr, &tex->view);
		if (FAILED(hr))
		{
			LIL_ERROR("Could not create texture resource view");
			delete tex;
			return nullptr;
		}

		//Sampler state desc
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = filter ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = wrap ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = wrap ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = wrap ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;

		//Create sampler state
		hr = ctx->device->CreateSamplerState(&samplerDesc, &tex->samplerState);
		if (FAILED(hr))
		{
			LIL_ERROR("Could not create sampler state");
			delete tex;
			return nullptr;
		}

		//Mip maps
		if (mipmaps)
			ctx->deviceContext->GenerateMips(tex->view);

		return tex;
	}

	void DX11Graphics::BindTexture(ITexture* texture, uint slot)
	{
		DX11Texture* tex = (DX11Texture*)texture;

		ctx->deviceContext->PSSetShaderResources(slot, 1, &tex->view);
		ctx->deviceContext->PSSetSamplers(slot, 1, &tex->samplerState);
	}

	void DX11Graphics::ReleaseTexture(ITexture** texture)
	{
		if (!texture)
			return;

		delete* texture;
		*texture = nullptr;
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
				LIL_ERROR("Shader compilation error: \n", (char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			else
				LIL_ERROR("Shader compilation failed. Could not recieve error message. Check the shader directory.");

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
			LIL_ERROR("Could not create input layout.");
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
