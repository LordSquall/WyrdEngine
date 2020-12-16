#pragma once

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void PhysicsComponent_SetIsTrigger(void* obj, bool isTrigger);

#ifdef __cplusplus
}
#endif