#include <Core/Math/LilMath.h>
#include "../IComponent.h"
#include "../Actor.h"
#include "TransformComponent.h"

namespace LilEngie
{
	void TransformComponent::Start()
	{
		actor->transform = this;
	}

	void TransformComponent::Update()
	{
		rotate(mat, vec3(0, 0, .01f));
	}
}
