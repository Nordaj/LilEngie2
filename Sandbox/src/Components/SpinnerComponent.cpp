#include "SpinnerComponent.h"

#include <LilEngie.h>

void SpinnerComponent::Update()
{
	//Frame speed dependent, dont really care
	actor->transform->euler += (rotSpeed * Game::core->deltaTime);
}
