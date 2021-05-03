#pragma once

/* local includes */
#include "../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void* GameObject_Create(void* parentGameObject, const char* name, bool retrieveManagedObject);

LIBRARY_API void GameObject_Destroy(void* gameObject);

LIBRARY_API const char* GameObject_Name_Get(void* obj);

LIBRARY_API int GameObject_UID_Get(void* obj);

LIBRARY_API void GameObject_Move(void* obj, float x, float y);

LIBRARY_API void GameObject_Rotate(void* obj, float angle);

LIBRARY_API void GameObject_Scale(void* obj, float x, float y);

LIBRARY_API void GameObject_SetAngle(void* obj, float angle);

LIBRARY_API void GameObject_SetVelocity(void* obj, float vX, float vY);

LIBRARY_API void GameObject_AddChild(void* obj, void* child);

LIBRARY_API int GameObject_Get_Component_Count(void* obj);

LIBRARY_API void GameObject_Get_Component(void* obj, int idx, int* type, void** componentPtr);


#ifdef __cplusplus
}
#endif