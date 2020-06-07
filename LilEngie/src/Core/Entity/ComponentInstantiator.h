#pragma once

#include <string>
#include <Core/Core.h>
#include <Core/Debug/Log.h>
#include "Actor.h"

#define LIL_COMPONENTS() \
namespace LilEngie { \
	class IComponent; \
	inline IComponent* CreateComponent(Actor* actor, const char* type) { 

//(unique component string id, class name)
#define LIL_COMPONENT_CREATOR(a,b) \
if (!strcmp(type, a)) return actor->CreateComponent<b>(); \

#define LIL_COMPONENTS_END() \
		LIL_ERROR("could not find a component id match. Please add all components to ComponentList.h."); \
		return nullptr; } } 
