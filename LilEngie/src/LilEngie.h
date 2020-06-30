#pragma once

#include <Core/Game/Game.h>
#include <Core/Graphics/DebugDrawing.h>
#include <Core/EventSystem/Events.h>
#include <Core/Debug/Log.h>
#include <Core/Debug/DebugTimer.h>
#include <Core/Entity/Scene.h>
#include <Core/Entity/Actor.h>
#include <Core/Entity/IComponent.h>
#include <Core/Entity/CoreComponents/TransformComponent.h>
#include <Core/Entity/CoreComponents/MeshComponent.h>
#include <Core/Entity/CoreComponents/CameraComponent.h>
#include <Core/Resources/Types/TextureResource.h>
#include <Core/Resources/Types/MaterialResource.h>
#include <Core/Resources/Types/ShaderResource.h>
#include <Core/Resources/Types/MeshResource.h>
#include <Core/Math/LilMath.h>

using namespace LilEngie;

#define LIL_ENGIE_SETUP(x, y, z) \
int main() \
{ \
	game = new Game(x, y, z); \
	game->Run(); \
	delete game; \
	return 0; \
}
