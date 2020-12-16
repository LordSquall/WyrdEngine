#pragma once

/* local includes */
#include "../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API int GameObject_Get_Component_Count(void* obj);

LIBRARY_API void GameObject_Get_Component(void* obj, int idx, int* type, void** componentPtr);

#ifdef __cplusplus
}
#endif