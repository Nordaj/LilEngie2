#pragma once

#include <vector>
#include <Core/Core.h>
#include "Vertex.h"

namespace LilEngie
{
	class IIndexBuffer;
	class IVertexBuffer;
	class IGraphics;

	class LIL_API Mesh
	{
	public:
		IVertexBuffer* vertexBuffer;
		IIndexBuffer* indexBuffer;
		uint indexCount;

	public:
		Mesh();
		~Mesh();

		bool Init(const std::vector<Vertex>& vertices, const std::vector<uint>& indices);
		bool IsInit();

		void Render(IGraphics* gfxContext = nullptr);
	};
}
