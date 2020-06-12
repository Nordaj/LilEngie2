#pragma once

#include "ComponentInstantiator.h"

#include "CoreComponents/TransformComponent.h"
#include "CoreComponents/CameraComponent.h"
#include "CoreComponents/MeshComponent.h"

LIL_CORE_COMPONENTS()

LIL_COMPONENT_CREATOR("transform", TransformComponent)
LIL_COMPONENT_CREATOR("camera", CameraComponent)
LIL_COMPONENT_CREATOR("mesh", MeshComponent)

LIL_CORE_COMPONENTS_END()
