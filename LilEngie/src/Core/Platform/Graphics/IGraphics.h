#pragma once

#include <string>
#include "InputLayout.h"
#include "Handles.h"
#include <Core/Core.h>

namespace LilEngie
{
	struct WinProp;

	enum GraphicsAPI
	{
		DirectX11,
		OpenGL
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
		virtual IVertexShader* CreateVertexShader(const std::string &file) = 0;
		virtual IVertexShader* CreateVertexShader(const std::string &file, IInputLayout** layout, InputElement* elements, uint numElements) = 0;
		virtual void SetVertexShader(IVertexShader* shader) = 0;
		virtual void ReleaseVertexShader(IVertexShader** shader) = 0;
		virtual IFragmentShader* CreateFragmentShader(const std::string &file) = 0;
		virtual void SetFragmentShader(IFragmentShader* shader) = 0;
		virtual void ReleaseFragmentShader(IFragmentShader** shader) = 0;
		virtual void SetInputLayout(IInputLayout* shader) = 0;
		virtual void ReleaseInputLayout(IInputLayout** layout) = 0;

		//Vertex and index buffers
		virtual IVertexBuffer* CreateVertexBuffer(float* verts, uint size) = 0;
		virtual void BindVertexBuffer(IVertexBuffer* vBuffer, uint stride) = 0;
		virtual void ReleaseVertexBuffer(IVertexBuffer** vBuffer) = 0;
		virtual IIndexBuffer* CreateIndexBuffer(uint* inds, uint size) = 0;
		virtual void BindIndexBuffer(IIndexBuffer* iBuffer) = 0;
		virtual void ReleaseIndexBuffer(IIndexBuffer** iBuffer) = 0;
		virtual void Draw(uint indexCount) = 0;

		static IGraphics* CreateGraphicsContext(GraphicsAPI api = DirectX11);
		static void ShutdownGraphicsContext(IGraphics** graphicsContext);

	private:
		virtual void Shutdown() = 0;
	};
}
