#pragma once

/* core wyrd includes */
#include "core/Structures.h"

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void* ScriptComponent_Create(void* obj);

LIBRARY_API void ScriptComponent_Enabled(void* obj, bool enable);

LIBRARY_API void ScriptComponent_CreateInstance(void* gameObject, void* component, const char* className);

#ifdef __cplusplus
}
#endif