#pragma once

/* core osiris includes */
#include "core/Structures.h"

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void Transform2D_SetPosition(void* obj, float x, float y);

LIBRARY_API void Transform2D_GetPosition(void* obj, Osiris::Vector2* vec);

LIBRARY_API void Transform2D_SetRotation(void* obj, float angle);

LIBRARY_API void Transform2D_GetRotation(void* obj, float* angle);

LIBRARY_API void Transform2D_SetScale(void* obj, float sx, float sy);

LIBRARY_API void Transform2D_GetScale(void* obj, Osiris::Vector2* vec);

#ifdef __cplusplus
}
#endif