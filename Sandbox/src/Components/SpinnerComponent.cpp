#include "SpinnerComponent.h"
#include <LilEngie.h>

void SpinnerComponent::Start()
{

}

void SpinnerComponent::Update()
{
	//Frame speed dependent, dont really care
	if (input->GetKey(Key::Space))
		actor->transform->euler += (rotSpeed * DeltaTime() * 1);

	actor->transform->euler += rotSpeed * input->GetMouseDelta().x * 100.1f * x;

	if (input->GetKeyUp(Key::Escape))
	{
		input->lockMousePos = !input->lockMousePos;
		input->MouseVisibility(!input->mouseVisible);
	}
}
