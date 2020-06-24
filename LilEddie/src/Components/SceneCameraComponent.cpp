#include <LilEngie.h>
#include "SceneCameraComponent.h"

namespace LilEddie
{
	void SceneCameraComponent::EditorUpdate()
	{
		if (input->GetMouseBtnDown(MouseButton::Right))
		{
			isLooking = true;

			input->lockMousePos = true;
			input->MouseVisibility(false);
		}

		if (isLooking)
		{
			//Looking behaviour
			vec3 e = input->GetMouseDelta();
			e = vec3(e.y, e.x, 0) * lookSpeed;
			actor->transform->euler += e;

			LIL_LOG(input->GetMouseDelta().x);

			//Moving behaviour
			mat4 rot = rotate(actor->transform->euler);
			vec3 forward = vec3(rot[2].x, rot[2].y, rot[2].z);
			vec3 right = vec3(rot[0].x, rot[0].y, rot[0].z);

			vec3 t;
			if (input->GetKey(Key::W))
				t += forward;
			if (input->GetKey(Key::S))
				t += forward * -1;
			if (input->GetKey(Key::D))
				t += right;
			if (input->GetKey(Key::A))
				t += right * -1;

			if (t.x || t.y || t.z)
				actor->transform->position += t / len(t) * DeltaTime() * moveSpeed;

			if (!input->GetMouseBtn(MouseButton::Right))
			{
				isLooking = false;

				input->lockMousePos = false;
				input->MouseVisibility(true);
			}
		}
	}
}
