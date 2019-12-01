#pragma once

#include "Core/Game/Game.h"
#include "Core/EventSystem/Events.h"
#include "Core/Debug/Log.h"
#include "Core/Debug/DebugTimer.h"
#include "Core/Entity/Scene.h"
#include "Core/Entity/Actor.h"
#include "Core/Entity/IComponent.h"

#define LIL_ENGIE_SETUP(x, y) int main() { LilEngie::Game game(x, y); return 0; }
