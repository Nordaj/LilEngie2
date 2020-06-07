#pragma once

#include <Core/Core.h>
#include <Core/System/ISerializable.h>
#include <Core/EventSystem/IEventListener.h>

namespace LilEngie
{
	class Actor;
	class SceneManager;

	class LIL_API IComponent : public IEventListener, public ISerializable
	{
		friend class Actor;
		friend class SceneManager;

	protected:
		Actor* actor;

	public:
		IComponent() {}
		virtual ~IComponent() {}

		//Destroys the component
		void Destroy();

	protected:
		virtual void Init() {}

		virtual void Start() {}
		virtual void Update() {}

		virtual void OnDraw() {}
	};
}
