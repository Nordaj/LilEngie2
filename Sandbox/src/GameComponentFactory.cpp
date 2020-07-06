#include "Components/SpinnerComponent.h"
#include "GameComponentFactory.h"

void GameComponentFactory::InitGameComponentList()
{
	componentNames.push_back({
		"spinner",
	});
}

IComponent* GameComponentFactory::CreateGameComponent(Actor* actor, const char* type)
{
	if (!strcmp(type, "spinner"))
		return actor->CreateComponent<SpinnerComponent>();
	return nullptr;
}
