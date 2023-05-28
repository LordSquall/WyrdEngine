#pragma once

/* local includes */
#include "core/ecs/ECS.h"
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API int32_t Material_SetPropTexture(void* nativePtr, char* name, void* texturePtr);

#ifdef __cplusplus
}
#endif