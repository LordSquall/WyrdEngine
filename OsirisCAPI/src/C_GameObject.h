#ifndef _C_GAMEOBJECT_H_
#define _C_GAMEOBJECT_H_

/* local includes */
#include "C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API const char* GameObject_Name_Get(void* obj);

LIBRARY_API void GameObject_Move(void* obj, float x, float y);

LIBRARY_API void GameObject_SetVelocity(void* obj, float vX, float vY);

#ifdef __cplusplus
}
#endif

#endif