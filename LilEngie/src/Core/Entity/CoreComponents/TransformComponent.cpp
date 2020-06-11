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

	}

	mat4 TransformComponent::GlobalTransformation()
	{
		mat4 m = LocalTransformation();
		for (Actor* p = actor->parent; p->parent; p = p->parent)
			m = p->transform->LocalTransformation() * m;
		return m;
	}

	mat4 TransformComponent::LocalTransformation()
	{
		return Math::scale(scale) * rotate(euler) * translate(position);
	}
}
