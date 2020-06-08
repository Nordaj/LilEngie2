#pragma once

#include <string>
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
		//Need to return string type identifier, ex: "transform"
		virtual std::string TypeName() = 0;

		virtual void Init() {}

		virtual void Start() {}
		virtual void Update() {}

		virtual void OnDraw() {}
	};
}

#define LIL_TYPE_NAME(n) std::string TypeName() override { return n; }
