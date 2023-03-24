#pragma once

/* local includes */
#include "core/ecs/ECS.h"
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API int32_t Texture_GetWidth(void* nativePtr);

LIBRARY_API int32_t Texture_GetHeight(void* nativePtr);

#ifdef __cplusplus
}
#endif