#pragma once

#include <LilEngie.h>

class SpinnerComponent : public IComponent
{
public:
	vec3 rotSpeed = {0, 0, 0};

	LIL_TYPE_NAME("spinner")
	PROPERTIES(
		PROPERTY(rotSpeed)
	)

public:
	void Update() override;
};
