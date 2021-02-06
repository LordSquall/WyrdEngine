#pragma once

/* core osiris includes */
#include "core/Structures.h"

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void* ScriptComponent_Create(void* obj, void* componentManagedObject);

LIBRARY_API void ScriptComponent_Enabled(void* obj, bool enable);

LIBRARY_API void ScriptComponent_CreateInstance(void* gameObject, void* component, const char* className);

#ifdef __cplusplus
}
#endif