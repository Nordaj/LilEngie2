#include <Core/Math/LilMath.h>
#include <Core/Entity/CoreComponents/CameraComponent.h>
#include <Core/Entity/CoreComponents/TransformComponent.h>
#include <Core/Entity/Actor.h>
#include "TransformGizmo.h"

namespace LilEngie
{
	void TransformGizmo::Init(Game* game, DebugDrawing* debugDrawing, CameraComponent* camera)
	{
		this->game = game;
		this->dbg = debugDrawing;
		this->camera = camera;
	}

	void TransformGizmo::Draw(vec3 mp)
	{
		//Make sure we are correctly initialized
		if (!game || !dbg || !camera)
			return;

		//Handle the appropriate gizmo
		switch (trsMode)
		{
			case TransformGizmoMode::Translate:
				DrawTranslate(mp);
				break;
			case TransformGizmoMode::Rotate:
				DrawRotate(mp);
				break;
			case TransformGizmoMode::Scale:
				DrawScale(mp);
				break;
			default:
				break;
		}
	}

	void TransformGizmo::DrawTranslate(vec3 mp)
	{
		//Transformation matrices
		mat4 m = selected->transform->GlobalTransformation();
		mat4 vp = camera->GetViewProjection();

		//Setup the rays
		vec3 ro = selected->transform->GlobalPosition();
		vec3 xRd = !worldMode ? normalized(m[0].xyz()) : vec3(1, 0, 0);
		vec3 yRd = !worldMode ? normalized(m[1].xyz()) : vec3(0, 1, 0);
		vec3 zRd = !worldMode ? normalized(m[2].xyz()) : vec3(0, 0, 1);

		//Get clip space rays
		vec4 cRo4 = vp * vec4(ro, 1);
		vec3 cRo = vec3(cRo4.x, cRo4.y, 0) / cRo4.w;

		vec4 cX4 = vp * vec4(xRd + ro, 1);
		vec3 cXRd = vec3(cX4.x, cX4.y, 0) / cX4.w - cRo;

		vec4 cY4 = vp * vec4(yRd + ro, 1);
		vec3 cYRd = vec3(cY4.x, cY4.y, 0) / cY4.w - cRo;

		vec4 cZ4 = vp * vec4(zRd + ro, 1);
		vec3 cZRd = vec3(cZ4.x, cZ4.y, 0) / cZ4.w - cRo;

		//Test for mouse hovering in clip space
		int h = 0;
		if (len(mp - cRo) < 0.05f)
			h = 1;
		else if (RayPointDistance(cRo, cXRd, mp) < 0.05f)
			h = -1;
		else if (RayPointDistance(cRo, cYRd, mp) < 0.05f)
			h = -2;
		else if (RayPointDistance(cRo, cZRd, mp) < 0.05f)
			h = -3;

		//Handle mouse down
		if (game->input.GetMouseBtnDown(MouseButton::Left))
		{
			selectedAxis = h;

			//Setup movement axis accordingly
			if (selectedAxis == 1)
				axis = normalized(camera->actor->transform->GlobalTransformation()[2].xyz());
			else if (selectedAxis < 0)
				axis = selectedAxis == -1 ? xRd : (selectedAxis == -2 ? yRd : zRd);
		}
		else if (!game->input.GetMouseBtn(MouseButton::Left))
			selectedAxis = 0;

		//Draw translation gizmo
		dbg->DrawLine(ro, ro + xRd, vec3(1, 0, 0), h == -1 ? 6 : 3, true);
		dbg->DrawLine(ro, ro + yRd, vec3(0, 1, 0), h == -2 ? 6 : 3, true);
		dbg->DrawLine(ro, ro + zRd, vec3(0, 0, 1), h == -3 ? 6 : 3, true);
		dbg->DrawSquare(ro, vec3(1, 1, 1), h == 1 ? 16 : 8, true);

		//Raycast from camera
		vec3 camRo, camRd;
		FrustrumRaycast(vp, mp, camRo, camRd);

		//Setup move target based on plane intersection or axis nearest
		vec3 pt;
		if (selectedAxis == 1)
			pt = camRo + (camRd * RayPlaneInt(camRo, camRd, selected->transform->GlobalPosition(), axis));
		else if (selectedAxis < 0)
			pt = RayRayNearest(selected->transform->GlobalPosition(), axis, camRo, camRd);

		//Move based on offset from beginning of grab
		if (selectedAxis != 0)
		{
			if (game->input.GetMouseBtnDown(MouseButton::Left))
				offset = pt - ro;

			selected->transform->SetGlobalPosition(pt - offset);
		}
	}

	void TransformGizmo::DrawRotate(vec3 mp)
	{
	}

	void TransformGizmo::DrawScale(vec3 mp)
	{
	}
}
