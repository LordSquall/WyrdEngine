#pragma once

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void Transform2D_SetPosition(void* obj, float x, float y);

LIBRARY_API void Transform2D_SetRotation(void* obj, float angle);

LIBRARY_API void Transform2D_SetScale(void* obj, float sx, float sy);

#ifdef __cplusplus
}
#endif