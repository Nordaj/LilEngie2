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
		~DX11Graphics();

		void Init(const WinProp &windowProperties) override;
		
		void SetClearColor(float r, float g, float b, float a) override;
		void Clear() override;
		void Render() override;
		void Resize(int width, int height) override;

		//Shaders and input layout
		IVertexShader* CreateVertexShader(const std::string &file) override;
		IVertexShader* CreateVertexShader(const std::string &file, IInputLayout** layout, InputElement* elements, uint numElements) override;
		void SetVertexShader(IVertexShader* shader) override;
		void ReleaseVertexShader(IVertexShader** shader) override;
		IFragmentShader* CreateFragmentShader(const std::string &file) override;
		void SetFragmentShader(IFragmentShader* shader) override;
		void ReleaseFragmentShader(IFragmentShader** shader) override;
		void SetInputLayout(IInputLayout* shader) override;
		void ReleaseInputLayout(IInputLayout** layout) override;

		//Vertex and index buffers
		IVertexBuffer* CreateVertexBuffer(float* verts, uint size) override;
		void BindVertexBuffer(IVertexBuffer* vBuffer, uint stride) override;
		void ReleaseVertexBuffer(IVertexBuffer** vBuffer) override;
		IIndexBuffer* CreateIndexBuffer(uint* inds, uint size) override;
		void BindIndexBuffer(IIndexBuffer* iBuffer) override;
		void ReleaseIndexBuffer(IIndexBuffer** iBuffer) override;
		void Draw(uint indexCount) override;

	private:
		void Shutdown() override;
		IInputLayout* CreateLayout(void* shaderBlob, InputElement* elements, uint numElements);
		void SetupRenderTargetView();
	};
}
