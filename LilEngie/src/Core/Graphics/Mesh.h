#pragma once

#include <vector>
#include <Core/Core.h>
#include "Vertex.h"

namespace LilEngie
{
	class IIndexBuffer;
	class IVertexBuffer;
	class IGraphics;
	class Renderer;

	class LIL_API Mesh
	{
	public:
		Renderer* renderer;
		IVertexBuffer* vertexBuffer;
		IIndexBuffer* indexBuffer;
		uint indexCount;

	private:
		IGraphics* gfx;

	public:
		Mesh();
		~Mesh();

		bool Init(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, IGraphics* gfx);
		bool IsInit();

		void Render();
	};
}
