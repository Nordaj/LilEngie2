#include "DirectX11/DX11Graphics.h"
#include "InputLayout.h"
#include "Handles.h"
#include "IGraphics.h"

namespace LilEngie
{
	IGraphics* IGraphics::CreateGraphicsContext(GraphicsAPI api)
	{
		switch (api)
		{
			case LilEngie::DirectX11:
			{
				DX11Graphics* g = new DX11Graphics();
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
