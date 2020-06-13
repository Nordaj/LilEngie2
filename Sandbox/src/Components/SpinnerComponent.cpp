#include "SpinnerComponent.h"

#include <LilEngie.h>

void SpinnerComponent::Update()
{
	//Frame speed dependent, dont really care
	if (GetKey(Key::Space))
		actor->transform->euler += (rotSpeed * Game::core->deltaTime);
}
