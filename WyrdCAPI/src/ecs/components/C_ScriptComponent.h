#pragma once

/* core wyrd includes */
#include <core/Structures.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void ScriptComponent_SetEnabled(void* obj, Wyrd::Entity entity, bool* position);

LIBRARY_API void ScriptComponent_SetScriptid(void* obj, Wyrd::Entity entity, Wyrd::UID* position);

LIBRARY_API void ScriptComponent_SetInstanceid(void* obj, Wyrd::Entity entity, int32_t* position);



#ifdef __cplusplus
}
#endif