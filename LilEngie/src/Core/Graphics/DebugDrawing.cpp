#include <vector>
#include <Core/Math/LilMath.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Graphics/Vertex.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Game/Game.h>
#include <Core/Resources/ResourceManager.h>
#include <Core/Debug/Log.h>
#include "Material.h"
#include "DebugDrawing.h"

namespace LilEngie
{
	DebugDrawing::DebugDrawing()
	{ }

	void DebugDrawing::Init(IGraphics* gfx, Game* game, Renderer* renderer)
	{
		this->gfx = gfx;
		this->game = game;
		this->renderer = renderer;

		//Quad mesh in clockwise order, face pointing in -z direction
		std::vector<Vertex> quadVerts = {
			{ vec4( .5, .5, 0, 1), vec4(0, 0, -1, 1), vec4(1, 0, 0, 1), 1, 0, 0, 0 }, //TOP RIGHT
			{ vec4( .5,-.5, 0, 1), vec4(0, 0, -1, 1), vec4(1, 0, 0, 1), 1, 1, 0, 0 }, //BOT RIGHT
			{ vec4(-.5,-.5, 0, 1), vec4(0, 0, -1, 1), vec4(1, 0, 0, 1), 0, 1, 0, 0 }, //BOT LEFT
			{ vec4(-.5, .5, 0, 1), vec4(0, 0, -1, 1), vec4(1, 0, 0, 1), 0, 0, 0, 0 }, //TOP LEFT
		};
		vBuffer = gfx->CreateVertexBuffer(&quadVerts[0].position[0], sizeof(Vertex) * 4);

		uint quadInds[] = {
			0, 1, 2,
			2, 3, 0
		};
		iBuffer = gfx->CreateIndexBuffer(quadInds, 6 * sizeof(uint));

		//Create shader as a resource
		std::string shader = "res/Shaders/Unlit";
		std::string path = game->resourceManager.GetResourcePath(shader);
		unlitShader = gfx->CreateShader(path + "VS", path + "FS");

		//Create plain white texture
		char col[4] = { 255, 255, 255, 255 };
		texture = gfx->CreateTexture(1, 1, TextureFormat::R8G8B8A8, col, true, false, false);

		//Create matrial
		MaterialProperty properties[] = {
			"color", MaterialPropertyType::VEC3
		};
		TextureProperty textures[] = {
			{"BaseColor", texture, 0}
		};
		material = new Material(unlitShader, properties, 1, textures, 1, renderer);
	}

	void DebugDrawing::Shutdown()
	{
		if (!gfx) return;

		gfx->ReleaseVertexBuffer(&vBuffer);
		gfx->ReleaseIndexBuffer(&iBuffer);
		gfx->ReleaseShader(&unlitShader);
		gfx->ReleaseTexture(&texture);
		delete material;
	}

	void DebugDrawing::RenderAll(int width, int height, float near, const mat4& v, const mat4& p)
	{
		//Dont render unless stuff was initialized
		if (!gfx) return;

		//Bind cbuffers
		gfx->BindCBuffer(renderer->cbPerCamera, ShaderType::Vertex, 1);
		gfx->BindCBuffer(renderer->cbPerObject, ShaderType::Vertex, 2);

		//Bind material
		material->BindMaterial();

		RenderLines(width, height, near, v, p, false);
		RenderSquares(width, height, near, v, p, false);

		gfx->DisableDepth();

		RenderLines(width, height, near, v, p, true);
		RenderSquares(width, height, near, v, p, true);

		gfx->EnableDepth();
	}

	void DebugDrawing::Flush()
	{
		lines.clear();
		squares.clear();

		overlayLines.clear();
		overlaySquares.clear();
	}

	void DebugDrawing::DrawLine(vec3 a, vec3 b, vec3 col, int pixelWidth, bool overlay)
	{
		if (overlay)
			overlayLines.push_back({ a, b, col, pixelWidth });
		else
			lines.push_back({a, b, col, pixelWidth});
	}

	void DebugDrawing::DrawSquare(vec3 p, vec3 col, int pixelWidth, bool overlay)
	{
		if (overlay)
			overlaySquares.push_back({ p, col, pixelWidth });
		else
			squares.push_back({ p, col, pixelWidth });
	}

	void DebugDrawing::RenderLines(int width, int height, float near, const mat4& v, const mat4& p, bool overlay)
	{
		//Get necessary variables
		mat4 vp = p * v;
		vec3 cam = inverse(v)[3].xyz();
		mat4 identity = mat4(1);
		vec3 pn = normalized(inverse(v)[2].xyz());
		vec3 po = inverse(v)[3].xyz();

		//Draw each line
		for (DebugLineDrawCall& line : overlay ? overlayLines : lines)
		{
			//Calc matrices
			mat4 m = mat4(1);

			///World space
			///Clip line by plane
			ClipLinePlane(line.a, line.b, po + (pn * near), pn);

			///Clip Space
			///Get a-b in clip space
			vec4 a = vp * vec4(line.a, 1);
			vec4 b = vp * vec4(line.b, 1);
			a /= a.w;
			b /= b.w;

			///Precalc translation to avoid aspect ratio scale
			vec3 t = (a.xyz() + b.xyz()) * .5;

			///Compensate for aspect ratio scale
			a.x *= width / (float)height;
			b.x *= width / (float)height;

			///Get length and dif
			vec3 d = b.xyz() - a.xyz();
			float l = len(d);

			///Line sizing
			scale(m, vec3(l, 2 * line.pixelWidth / (float)height, 1));

			///Rotate line to points
			vec3 right = normalized(d);
			vec3 forward = -vec3(0, 0, 1); //not perp
			vec3 up = cross(right, forward);
			mat4 lr = mat4(1);
			lr[0] = right;
			lr[1] = up;
			lr[2] = forward;
			m = lr * m;

			///Scale horizontally for aspect ratio
			scale(m, vec3(height / (float)width, 1, 1));

			///Translate line to points
			translate(m, t);

			//Update cbuffers
			void* perCamera = gfx->GetCBufferPtr(renderer->cbPerCamera);
			memcpy(perCamera, &identity, sizeof(mat4));
			gfx->UpdateCBuffer(renderer->cbPerCamera);

			void* perObject = gfx->GetCBufferPtr(renderer->cbPerObject);
			memcpy(perObject, &m, sizeof(mat4));
			gfx->UpdateCBuffer(renderer->cbPerObject);

			//Update material
			material->SetProperty("color", line.col);
			material->UpdateProperties();

			//Draw
			gfx->BindIndexBuffer(iBuffer);
			gfx->BindVertexBuffer(vBuffer, sizeof(Vertex));
			gfx->Draw(6);
		}
	}

	void DebugDrawing::RenderSquares(int width, int height, float, const mat4& v, const mat4& p, bool overlay)
	{
		//Get necessary variables
		mat4 vp = p * v;
		vec3 cam = inverse(v)[3].xyz();
		mat4 identity = mat4(1);
		vec3 pn = normalized(inverse(v)[2].xyz());
		vec3 po = inverse(v)[3].xyz();

		//Draw each square
		for (DebugSquareDrawCall& square : overlay ? overlaySquares : squares)
		{
			//Calc matrices (everyting done in clip space)
			mat4 camVP = mat4(1);
			mat4 m = mat4(1);

			///Calculate square clip space center
			vec4 c = vp * vec4(square.c, 1);
			c /= c.w;

			///Scale square width/height basedon height
			float s = square.pixelWidth * 2 / (float)height;
			scale(m, vec3(s, s, 1));

			///Adjust the x size for inverse aspect ratio
			scale(m, vec3((height / (float)width), 1, 1));

			///Translate to center
			translate(m, c.xyz());

			//Update cbuffers
			void* perCamera = gfx->GetCBufferPtr(renderer->cbPerCamera);
			memcpy(perCamera, &camVP, sizeof(mat4));
			gfx->UpdateCBuffer(renderer->cbPerCamera);

			void* perObject = gfx->GetCBufferPtr(renderer->cbPerObject);
			memcpy(perObject, &m, sizeof(mat4));
			gfx->UpdateCBuffer(renderer->cbPerObject);

			//Update material
			material->SetProperty("color", square.col);
			material->UpdateProperties();

			//Draw
			gfx->BindIndexBuffer(iBuffer);
			gfx->BindVertexBuffer(vBuffer, sizeof(Vertex));
			gfx->Draw(6);
		}
	}
}
