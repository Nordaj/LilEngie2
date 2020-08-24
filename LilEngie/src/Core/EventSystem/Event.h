#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	enum EventType
	{
		WindowResize,
		WindowClose,
		GameClose,
		SceneUnload,
		EnterPlayMode,
		ExitPlayMode,
		EditorSceneRecall,
		EventTypeCount //Amount of unique types
	};

	class LIL_API EventArg
	{
	public:
		union
		{
			float asFloat;
			int asInt;
			bool asBool;
			void* asPtr;
		};
	};

	class LIL_API Event
	{
	public:
		EventType type;
		unsigned int argsCount;
		//4 is maximum event argument size for now
		EventArg args[4];

		Event() { }
		Event(EventType type)
			:type(type) { }
	};
}
