#pragma once

#include "Core/Game/Game.h"
#include "Core/Game/ServiceLocator.h"
#include "Core/EventSystem/Events.h"
#include "Core/Debug/Log.h"

#define LIL_ENGIE_SETUP(x, y) int main() { LilEngie::Game game(x, y); return 0; }
