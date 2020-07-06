#pragma once

#include <LilEngie.h>

class GameComponentFactory : public ComponentFactory
{
protected:
	void InitGameComponentList() override;
	IComponent* CreateGameComponent(Actor* actor, const char* type) override;
};
