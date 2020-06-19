#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <Core/Core.h>
#include <Core/Debug/Log.h>
#include <Core/Graphics/Mesh.h>
#include <Core/Graphics/Vertex.h>
#include <Core/Game/Game.h>
#include <Core/Graphics/Renderer.h>
#include "../ResourceManager.h"
#include "MeshResource.h"

namespace LilEngie
{
	MeshResource::MeshResource(const ResourceId& resourceId, ResourceManager* manager, bool load)
		: IResource(resourceId, manager)
	{ 
		if (load)
			LoadMesh();
	}

	MeshResource::~MeshResource()
	{ }

	bool MeshResource::LoadMesh()
	{
		//Load into importer (currently not completely safe)
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(manager->GetResourcePath(resourceId), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LIL_ERROR("Could not load mesh...");
			return false;
		}

		//Get first mesh
		aiMesh* mesh = scene->mMeshes[0];

		//Fill vertex data
		std::vector<Vertex> verts = std::vector<Vertex>(mesh->mNumVertices);
		for (int i = 0; i < verts.size(); i++)
		{
			if (mesh->mNumVertices > 0)
				verts[i].position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1 };

			if (mesh->HasNormals())
				verts[i].normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 1 };

			if (mesh->HasTangentsAndBitangents())
				verts[i].tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1 };

			if (mesh->HasTextureCoords(0))
			{
				verts[i].u = mesh->mTextureCoords[0][i].x;
				verts[i].v = mesh->mTextureCoords[0][i].y;
			}
		}

		//Fill index data
		std::vector<uint> inds = std::vector<uint>(mesh->mNumFaces * 3);
		for (int i = 0; i < mesh->mNumFaces; i++)
		{
			inds[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
			inds[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			inds[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		//Actually init mesh
		IGraphics* gfx = manager->game->renderer.gfx;
		return this->mesh.Init(verts, inds, gfx);
	}

	bool MeshResource::IsLoaded()
	{
		return mesh.IsInit();
	}
}
