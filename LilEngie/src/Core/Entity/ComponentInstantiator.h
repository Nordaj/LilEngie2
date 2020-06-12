#pragma once

#include <string>
#include <map>
#include <Core/Core.h>
#include <Core/Debug/Log.h>
#include "Actor.h"

#define LIL_CORE_COMPONENTS() \
namespace LilEngie { \
	class IComponent; \
	inline IComponent* CreateComponentFromString(Actor* actor, std::string type) { 

#define LIL_GAME_COMPONENTS() \
	inline IComponent* CreateComponentFromString(Actor* actor, std::string type) { 

//(unique component string id, class name)
#define LIL_COMPONENT_CREATOR(a,b) \
if (!strcmp(type.c_str(), a)) return actor->CreateComponent<b>(); \

#define LIL_GAME_COMPONENTS_END() \
		LIL_ERROR("could not find a component id match. Please add all components to ComponentList.h."); \
		return nullptr; } 

#define LIL_CORE_COMPONENTS_END() \
		return nullptr; } } 
