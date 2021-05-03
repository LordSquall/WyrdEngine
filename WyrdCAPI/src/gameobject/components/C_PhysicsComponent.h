#pragma once

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void* PhysicsComponent_Create(void* obj);

LIBRARY_API void PhysicsComponent_SetIsTrigger(void* obj, bool isTrigger);

LIBRARY_API bool PhysicsComponent_GetIsTrigger(void* obj);

LIBRARY_API void PhysicsComponent_SetIsStatic(void* obj, bool isStatic);

LIBRARY_API bool PhysicsComponent_GetIsStatic(void* obj);

#ifdef __cplusplus
}
#endif