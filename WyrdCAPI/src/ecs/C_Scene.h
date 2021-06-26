#pragma once

/* local includes */
#include "core/ecs/ECS.h"
#include "../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void Scene_BuildComponentList(void* scenePtr);

LIBRARY_API void* Scene_GetComponent(void* scenePtr, uint32_t componentPoolIndex, Wyrd::Entity entity);

#ifdef __cplusplus
}
#endif