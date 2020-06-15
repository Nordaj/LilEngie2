#include "SpinnerComponent.h"
#include <Vendor/imgui/imgui.h>
#include <LilEngie.h>

void SpinnerComponent::Start()
{

}

void SpinnerComponent::Update()
{
	//Frame speed dependent, dont really care
	if (input->GetKey(Key::Space))
		actor->transform->euler += (rotSpeed * Game::core->deltaTime);

	actor->transform->euler += rotSpeed * input->GetMouseDelta().x * 0.001f * x;

	if (input->GetKeyUp(Key::Escape))
	{
		input->lockMousePos = !input->lockMousePos;
		input->MouseVisibility(!input->mouseVisible);
	}
}

void SpinnerComponent::OnDrawImGui()
{
	//Draw imgui stuff
	ImGui::Begin("window from spinner");
	ImGui::SliderFloat("Spinner Speed", &x, 0, 3);
	ImGui::End();
}
