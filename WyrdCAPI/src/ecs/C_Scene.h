#pragma once

/* local includes */
#include "core/ecs/ECS.h"
#include "../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void Scene_BuildComponentList(void* scenePtr);

LIBRARY_API void* Scene_GetComponent(void* scenePtr, uint32_t componentPoolIndex, Wyrd::Entity entity);

LIBRARY_API void* Scene_AddComponent(void* scenePtr, uint32_t componentPoolIndex, Wyrd::Entity entity);

LIBRARY_API Wyrd::Entity Scene_CreateEntity(void* scenePtr, const char* name);

#ifdef __cplusplus
}
#endif