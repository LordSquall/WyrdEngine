#pragma once

/* local includes */
#include "core/ecs/ECS.h"
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API INT32 Texture_GetWidth(void* nativePtr);

LIBRARY_API INT32 Texture_GetHeight(void* nativePtr);

LIBRARY_API BSTR Texture_GetName(void* nativePtr);

#ifdef __cplusplus
}
#endif