#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Debug/Log.h>
#include <Core/Graphics/Vertex.h>
#include "Renderer.h"
#include "Mesh.h"

namespace LilEngie
{
	Mesh::Mesh()
		: vertexBuffer(nullptr), indexBuffer(nullptr), indexCount(0)
	{ }

	Mesh::~Mesh()
	{ 
		if (vertexBuffer)
			gfx->ReleaseVertexBuffer(&vertexBuffer);

		if (indexBuffer)
			gfx->ReleaseIndexBuffer(&indexBuffer);
	}

	bool Mesh::Init(const std::vector<Vertex>& vertices, const std::vector<uint>& indices, IGraphics* gfx)
	{
		//Make sure not currently init
		if (IsInit())
			return false;

		//Get graphics context
		this->gfx = gfx;

		//Create bufers
		vertexBuffer = gfx->CreateVertexBuffer((float*)&vertices[0], sizeof(Vertex) * vertices.size());
		indexCount = indices.size();
		uint* ptr = (uint*)&(indices[0]);
		indexBuffer = gfx->CreateIndexBuffer((uint*)&indices[0], sizeof(uint) * indexCount);

		//Make sure it succeeded
		if (!vertexBuffer || !indexBuffer)
		{
			LIL_ERROR("Could not create vertex or index buffer for mesh.");
			vertexBuffer = nullptr;
			indexBuffer = nullptr;
			return false;
		}
		return true;
	}

	bool Mesh::IsInit()
	{
		return indexCount > 0;
	}

	void Mesh::Render()
	{
		gfx->BindVertexBuffer(vertexBuffer, sizeof(Vertex));
		gfx->BindIndexBuffer(indexBuffer);

		gfx->Draw(indexCount);
	}
}
