#pragma once

#include <Core/Core.h>
#include <Core/Math/LilMath.h>
#include "../IComponent.h"

namespace LilEngie
{
	class LIL_API CameraComponent : public IComponent
	{
	public:
		mat4 view;
		mat4 projection;

		void Update() override;
		void OnDraw() override;
	};
}
