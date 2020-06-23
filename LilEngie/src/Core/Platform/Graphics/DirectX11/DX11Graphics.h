#pragma once

#include <Core/Common.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>

namespace LilEngie
{
	struct D3D11Ctx;
	struct WinProp;

	class DX11Graphics : public IGraphics
	{
	private:
		D3D11Ctx* ctx;
		float clearColor[4];

	public:
		~DX11Graphics() override;

		void Init(const WinProp &windowProperties) override;
		
		void SetClearColor(float r, float g, float b, float a) override;
		void Clear() override;
		void Render() override;
		void Resize(int width, int height) override;
		void SetContextCurrent() override {}

		//Shaders and input layout
		IShader* CreateShader(const std::string &vert, const std::string &frag) override;
		IShader* CreateShader(const std::string &vert, const std::string &frag, IInputLayout** layout, InputElement* elements, uint numElements) override;
		void SetShader(IShader* shader) override;
		void ReleaseShader(IShader** shader) override;
		void SetInputLayout(IInputLayout* layout) override;
		void ReleaseInputLayout(IInputLayout** layout) override;

		//Vertex and index buffers
		IVertexBuffer* CreateVertexBuffer(float* verts, uint size) override;
		void BindVertexBuffer(IVertexBuffer* vBuffer, uint stride) override;
		void ReleaseVertexBuffer(IVertexBuffer** vBuffer) override;
		IIndexBuffer* CreateIndexBuffer(uint* inds, uint size) override;
		void BindIndexBuffer(IIndexBuffer* iBuffer) override;
		void ReleaseIndexBuffer(IIndexBuffer** iBuffer) override;
		void Draw(uint indexCount) override;

		//Constant buffers
		ICBuffer* CreateCBuffer(uint size, void* initData = nullptr) override;
		void* GetCBufferPtr(ICBuffer* cBuffer) override;
		uint GetCBufferSize(ICBuffer* cBuffer) override;
		void UpdateCBuffer(ICBuffer* cBuffer) override;
		void BindCBuffer(ICBuffer* cBuffer, ShaderType type, uint slot) override;
		void ReleaseCBuffer(ICBuffer** cBuffer) override;

		//Textures
		ITexture* CreateTexture(uint width, uint height, TextureFormat format, void* data, bool wrap = true, bool mipmaps = true, bool filter = true) override;
		void BindTexture(ITexture* texture, uint slot) override;
		void ReleaseTexture(ITexture** texture) override;

		//Framebuffers
		IFramebuffer* CreateFramebuffer(int width, int height) override;
		void BindFramebuffer(IFramebuffer* framebuffer) override;
		void UnbindFramebuffer() override;
		ITexture* GetFramebufferTexture(IFramebuffer* framebuffer, bool depth = false) override;
		void ReleaseFramebuffer(IFramebuffer** framebuffer) override;

		//Dear ImGUI
		void ImGuiInit(const WinProp& windowProperties) override;
		void ImGuiNewFrame() override;
		void ImGuiRender() override;
		void ImGuiShutdown() override;
		void* ImGuiGetTex(ITexture* tex) override;

	private:
		void Shutdown() override;
		IInputLayout* CreateLayout(void* shaderBlob, InputElement* elements, uint numElements);
		void SetupDepthStencil(int width, int height);
		void SetupRenderTargetView();
		void HandleDebugMessages();
	};
}
