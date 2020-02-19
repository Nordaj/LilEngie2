#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	class Actor;

	class LIL_API IComponent
	{
		friend class Actor;

	private:
		Actor* actor;

	public:
		IComponent() {}
		virtual ~IComponent() {}

		//Destroys the component
		void Destroy();

	protected:
		virtual void Start() {}
		virtual void Update() {}

		virtual void OnDraw() {}
	};
}
