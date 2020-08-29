#include <Core/Math/LilMath.h>
#include "../IComponent.h"
#include "../Actor.h"
#include "TransformComponent.h"

namespace LilEngie
{
	void TransformComponent::Init()
	{
		actor->transform = this;
	}

	void TransformComponent::SetGlobalPosition(vec3 pos)
	{
		position = (inverse(InheritedTransformation()) * vec4(pos, 1)).xyz();
	}

	//TODO
	void TransformComponent::SetGlobalEuler(vec3 euler)
	{
	}

	void TransformComponent::SetGlobalScale(vec3 scale)
	{
		vec3 s = GlobalScale();
		this->scale = scale / s;
	}

	vec3 TransformComponent::GlobalPosition()
	{
		return GlobalTransformation()[3].xyz();
	}

	//TODO
	vec3 TransformComponent::GlobalEuler()
	{
		return vec3();
	}

	vec3 TransformComponent::GlobalScale()
	{
		vec3 scale = this->scale;
		for (Actor* p = actor->parent; p->parent && p->transform; p = p->parent)
			scale *= p->transform->scale;
		return scale;
	}

	mat4 TransformComponent::InheritedTransformation()
	{
		mat4 m = mat4(1);
		for (Actor* p = actor->parent; p->parent && p->transform; p = p->parent)
			m = p->transform->LocalTransformation() * m;
		return m;
	}

	mat4 TransformComponent::GlobalTransformation()
	{
		return InheritedTransformation() * LocalTransformation();
	}

	mat4 TransformComponent::LocalTransformation()
	{
		return translate(position) * rotate(euler) * Math::scale(scale);
	}
}
