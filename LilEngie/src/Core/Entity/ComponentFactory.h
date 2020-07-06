#pragma once

#include <string>
#include <map>
#include <Core/Core.h>
#include <Core/Debug/Log.h>
#include "Actor.h"

namespace LilEngie
{
	class LIL_API ComponentFactory
	{
	public:
		static ComponentFactory* core;
		std::vector<std::string> componentNames;

	public:
		void InitComponentList();
		IComponent* CreateComponent(Actor* actor, const char* type);

	protected:
		virtual void InitGameComponentList();
		virtual IComponent* CreateGameComponent(Actor* actor, const char* type);
	};
}
