#include "SpinnerComponent.h"

#include <LilEngie.h>

void SpinnerComponent::Start()
{
	input->lockMousePos = true;
	input->MouseVisibility(false);
}

void SpinnerComponent::Update()
{
	//Frame speed dependent, dont really care
	if (input->GetKey(Key::Space))
		actor->transform->euler += (rotSpeed * Game::core->deltaTime);

	actor->transform->euler += rotSpeed * input->GetMouseDelta().x * 0.001f;

	if (input->GetKeyUp(Key::Escape))
	{
		input->lockMousePos = !input->lockMousePos;
		input->MouseVisibility(!input->mouseVisible);
	}
}
