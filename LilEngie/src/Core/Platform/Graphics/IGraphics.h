#pragma once

#include <string>
#include "InputLayout.h"
#include "Handles.h"
#include <Core/Core.h>

namespace LilEngie
{
	struct WinProp;

	enum class GraphicsAPI
	{
		DirectX11,
		OpenGL
	};

	enum class ShaderType 
	{ 
		Fragment, 
		Vertex 
	};

	enum class TextureFormat
	{
		R8,
		R8G8,
		R8G8B8A8,
		D24S8
	};

	class LIL_API IGraphics
	{
	private:
		GraphicsAPI graphicsAPI;

	public:
		IGraphics() {}
		virtual ~IGraphics() {}

		virtual void Init(const WinProp &windowProperties) = 0;

		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear() = 0;
		virtual void Render() = 0;
		virtual void Resize(int width, int height) = 0;
		virtual void SetContextCurrent() = 0;

		//Shaders and input layout
		virtual IShader* CreateShader(const std::string& vert, const std::string& frag) = 0;
		virtual IShader* CreateShader(const std::string &vert, const std::string &frag, IInputLayout** layout, InputElement* elements, uint numElements) = 0;
		virtual void SetShader(IShader* shader) = 0;
		virtual void ReleaseShader(IShader** shader) = 0;
		virtual void SetInputLayout(IInputLayout* layout) = 0;
		virtual void ReleaseInputLayout(IInputLayout** layout) = 0;

		//Vertex and index buffers
		virtual IVertexBuffer* CreateVertexBuffer(float* verts, uint size) = 0;
		virtual void BindVertexBuffer(IVertexBuffer* vBuffer, uint stride) = 0;
		virtual void ReleaseVertexBuffer(IVertexBuffer** vBuffer) = 0;
		virtual IIndexBuffer* CreateIndexBuffer(uint* inds, uint size) = 0;
		virtual void BindIndexBuffer(IIndexBuffer* iBuffer) = 0;
		virtual void ReleaseIndexBuffer(IIndexBuffer** iBuffer) = 0;
		virtual void Draw(uint indexCount) = 0;

		//Constant buffers
		virtual ICBuffer* CreateCBuffer(uint size, void* initData = nullptr) = 0;
		virtual void* GetCBufferPtr(ICBuffer* cBuffer) = 0;
		virtual uint GetCBufferSize(ICBuffer* cBuffer) = 0;
		virtual void UpdateCBuffer(ICBuffer* cBuffer) = 0;
		virtual void BindCBuffer(ICBuffer* cBuffer, ShaderType type, uint slot) = 0;
		virtual void ReleaseCBuffer(ICBuffer** cBuffer) = 0;

		//Textures
		virtual ITexture* CreateTexture(uint width, uint height, TextureFormat format, void* data, bool wrap = true, bool mipmaps = true, bool filter = true) = 0;
		virtual void BindTexture(ITexture* texture, uint slot) = 0;
		virtual void ReleaseTexture(ITexture** texture) = 0;

		//Framebuffers
		virtual IFramebuffer* CreateFramebuffer(int width, int height) = 0;
		virtual void BindFramebuffer(IFramebuffer* framebuffer) = 0;
		virtual void UnbindFramebuffer() = 0;
		virtual ITexture* GetFramebufferTexture(IFramebuffer* framebuffer, bool depth = false) = 0;
		virtual void GetFramebufferSize(IFramebuffer* framebuffer, int* width, int* height) = 0;
		virtual void ReleaseFramebuffer(IFramebuffer** framebuffer) = 0;

		//Dear ImGUI
		virtual void ImGuiInit(const WinProp& windowProperties) = 0;
		virtual void ImGuiNewFrame() = 0;
		virtual void ImGuiRender() = 0;
		virtual void ImGuiShutdown() = 0;
		virtual void* ImGuiGetTex(ITexture* tex) = 0;

		static IGraphics* CreateGraphicsContext(GraphicsAPI api = GraphicsAPI::DirectX11);
		GraphicsAPI GetGraphicsAPI();
		static void ShutdownGraphicsContext(IGraphics** graphicsContext);

	private:
		virtual void Shutdown() = 0;
	};
}
