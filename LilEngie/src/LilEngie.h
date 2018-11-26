#pragma once

#include "Core/Game/Game.h"
#include "Core/Application/Entry.h"

#define SET_METHODS(x, y) std::function<void()> LilEngie::Entry::start = x; std::function<void()> LilEngie::Entry::update = y;
#define LIL_ENGIE_SETUP(x, y) SET_METHODS(x, y) int main() { LilEngie::Game game = LilEngie::Game(LilEngie::Entry::start, LilEngie::Entry::update); return 0; }
