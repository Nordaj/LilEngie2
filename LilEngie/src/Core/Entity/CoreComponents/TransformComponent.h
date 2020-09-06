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
		quat rotation;
		vec3 scale = vec3(1, 1, 1);

	private:
		LIL_TYPE_NAME("transform")
		PROPERTIES(
			PROPERTY(position)
			PROPERTY(rotation)
			PROPERTY(scale)
		)

	public:
		void Init() override;

		void SetGlobalPosition(vec3 position);
		void SetGlobalEuler(vec3 euler);
		void SetGlobalScale(vec3 scale);

		vec3 GlobalPosition();
		vec3 GlobalEuler();
		vec3 GlobalScale();

		mat4 InheritedTransformation();
		mat4 GlobalTransformation();
		mat4 LocalTransformation();
	};
}
