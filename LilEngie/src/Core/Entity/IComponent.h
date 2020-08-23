#pragma once

#include <string>
#include <Core/Core.h>
#include <Core/System/ISerializable.h>
#include <Core/EventSystem/IEventListener.h>
#include <Core/Platform/Window/Keys.h>
#include <Core/Platform/Window/MouseState.h>
#include <Core/Game/Input.h>

namespace LilEngie
{
	class Actor;
	class SceneManager;

	class LIL_API IComponent : public IEventListener, public ISerializable
	{
		friend class Actor;
		friend class SceneManager;

	public:
		Actor* actor;
		Input* input;

	public:
		IComponent();
		virtual ~IComponent() {}

		//Destroys the component
		void Destroy();

		//Need to return string type identifier, ex: "transform"
		virtual std::string TypeName() = 0;

		virtual bool CheckDependencies() { return true; }
		virtual void Init() {}

		virtual void Start() {}
		virtual void Update() {}
		virtual void EditorUpdate() {}

		virtual void OnDraw() {}
		virtual void OnDrawImGui() {}

		Actor* GetActor(std::string& uid);

		float DeltaTime();
	};
}

#define LIL_TYPE_NAME(n) std::string TypeName() override { return n; }
#define LIL_DEPENDENCIES(...) \
bool CheckDependencies() override { \
	std::vector<std::string> ids = {__VA_ARGS__}; \
	return actor->ContainsComponents(ids); \
}
