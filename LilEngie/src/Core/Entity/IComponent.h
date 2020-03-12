#pragma once

#include <Core/Core.h>
#include <Core/EventSystem/IEventListener.h>

namespace LilEngie
{
	class Actor;

	class LIL_API IComponent : public IEventListener
	{
		friend class Actor;

	protected:
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
