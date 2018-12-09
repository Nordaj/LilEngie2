#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	class LIL_API Component
	{
		friend class Actor;

	private:
		Actor* actor;

	public:
		//Destroys the component
		void Destroy();

	protected:
		virtual void Start() {}
		virtual void Update() {}
	};
}
