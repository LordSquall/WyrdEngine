#pragma once

/* local includes */
#include "../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API const char* GameObject_Name_Get(void* obj);

LIBRARY_API void GameObject_Move(void* obj, float x, float y);

LIBRARY_API void GameObject_Rotate(void* obj, float angle);

LIBRARY_API void GameObject_Scale(void* obj, float x, float y);

LIBRARY_API void GameObject_SetVelocity(void* obj, float vX, float vY);

LIBRARY_API void GameObject_AddChild(void* obj, void* child);

#ifdef __cplusplus
}
#endif