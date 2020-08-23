#include <LilEngie.h>
#include "SceneCameraComponent.h"

namespace LilEddie
{
	void SceneCameraComponent::Init()
	{
		dbg = &actor->game->renderer.debugDrawing;
	}

	void SceneCameraComponent::OnDraw()
	{
		DrawSceneLines();
	}

	void SceneCameraComponent::EditorUpdate()
	{
		if (input->GetMouseBtnDown(MouseButton::Right) && hovered)
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

	void SceneCameraComponent::DrawSceneLines()
	{
		if (!dbg) return;

		//Axis lines
		dbg->DrawLine(vec3(0, 0, 0), vec3(1, 0, 0), vec3(1, 0, 0), 2);
		dbg->DrawLine(vec3(0, 0, 0), vec3(0, 1, 0), vec3(0, 1, 0), 2);
		dbg->DrawLine(vec3(0, 0, 0), vec3(0, 0, 1), vec3(0, 0, 1), 2);

		//Grid lines
		int n = 50;
		int hn = n / 2;
		vec3 c = vec3(1, .3, .7) * .5;
		for (int i = 1; i < n; i++)
		{
			dbg->DrawLine(vec3(i - hn, 0, -hn), vec3(i - hn, 0, hn), c, 1);
		}

		for (int k = 1; k < n; k++)
		{
			dbg->DrawLine(vec3(-hn, 0, k - hn), vec3(hn, 0, k - hn), c, 1);
		}
	}
}
