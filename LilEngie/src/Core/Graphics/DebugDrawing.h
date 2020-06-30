#pragma once

#include <vector>
#include <Core/Math/LilMath.h>
#include <Core/Core.h>

namespace LilEngie
{
	class IGraphics;
	class Game;
	class Renderer;
	class IShader;
	class IIndexBuffer;
	class IVertexBuffer;
	class ITexture;
	class Material;

	struct DebugLineDrawCall
	{
		vec3 a, b;
		vec3 col;
		int pixelWidth;
	};

	struct DebugSquareDrawCall
	{
		vec3 c;
		vec3 col;
		int pixelWidth;
	};

	class LIL_API DebugDrawing
	{
	private:
		Game* game = nullptr;
		IGraphics* gfx = nullptr;
		Renderer* renderer = nullptr;

		IVertexBuffer* vBuffer = nullptr;
		IIndexBuffer* iBuffer = nullptr;
		IShader* unlitShader = nullptr;
		ITexture* texture = nullptr;
		Material* material = nullptr;

		std::vector<DebugLineDrawCall> lines;
		std::vector<DebugSquareDrawCall> squares;

	public:
		DebugDrawing();

		void Init(IGraphics* gfx, Game* game, Renderer* renderer);
		void Shutdown();

		//Finally draws everything only now
		void RenderAll(int width, int height, float near, const mat4& v, const mat4& p);
		void Flush();

		//Caches every draw call
		void DrawLine(vec3 a, vec3 b, vec3 col, int pixelWidth);
		void DrawSquare(vec3 c, vec3 col, int pixelWidth);
	};
}
