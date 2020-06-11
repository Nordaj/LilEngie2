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
		vec3 position;
		vec3 euler;
		vec3 scale;

		LIL_TYPE_NAME("transform")
		PROPERTIES(
			PROPERTY(position)
			NAMED_PROPERTY(euler, "rotation")
			PROPERTY(scale)
		)

	public:
		void Start() override;
		void Update() override;

		mat4 GlobalTransformation();
		mat4 LocalTransformation();
	};
}
