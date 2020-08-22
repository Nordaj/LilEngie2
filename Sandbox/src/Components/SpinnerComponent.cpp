#include "SpinnerComponent.h"
#include <LilEngie.h>

void SpinnerComponent::Start()
{

}

void SpinnerComponent::Update()
{
	if (input->GetKey(Key::Space))
		actor->transform->euler += (rotSpeed * DeltaTime() * 1);

	if (input->GetKeyUp(Key::Escape))
	{
		input->lockMousePos = !input->lockMousePos;
		input->MouseVisibility(!input->mouseVisible);
	}
}
