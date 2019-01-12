#pragma once

#include <Core/Common.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>

namespace LilEngie
{
	struct GLCtx;
	struct WinProp;

	class GLGraphics : public IGraphics
	{
	private:
		GLCtx* ctx;
		float clearColor[4];

	public:
		~GLGraphics() override;

		void Init(const WinProp &windowProperties) override;

		void SetClearColor(float r, float g, float b, float a) override;
		void Clear() override;
		void Render() override;
		void Resize(int width, int height) override;
		void SetContextCurrent() override;

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

	private:
		void Shutdown() override;
		IInputLayout* CreateLayout(InputElement* elements, uint numElements);
		uint CompileShader(const std::string &src, int type);
	};
}