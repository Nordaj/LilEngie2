#pragma once

#include <string>
#include <Core/Core.h>
#include <Core/Math/LilMath.h>
#include "../IComponent.h"

namespace LilEngie
{
	class LIL_API TransformComponent : public IComponent
	{
	public:
		mat4 mat;

		LIL_TYPE_NAME("transform")
		PROPERTIES(
			PROPERTY(mat)
		)

	public:
		void Start() override;
		void Update() override;
	};
}
