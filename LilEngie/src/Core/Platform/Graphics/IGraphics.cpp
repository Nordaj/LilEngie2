#include "DirectX11/DX11Graphics.h"
#include "OpenGL/GLGraphics.h"
#include "InputLayout.h"
#include "Handles.h"
#include "IGraphics.h"

namespace LilEngie
{
	IGraphics* IGraphics::CreateGraphicsContext(GraphicsAPI api)
	{
		switch (api)
		{
			case GraphicsAPI::DirectX11:
			{
				DX11Graphics* g = new DX11Graphics();
				g->graphicsAPI = api;
				return g;
			}
			case GraphicsAPI::OpenGL:
			{
				GLGraphics* g = new GLGraphics();
				g->graphicsAPI = api;
				return g;
			}
			default:
				return nullptr;
		}
	}

	void IGraphics::ShutdownGraphicsContext(IGraphics** graphicsContext)
	{
		(*graphicsContext)->Shutdown();
		delete *graphicsContext;
		*graphicsContext = nullptr;
	}
}
