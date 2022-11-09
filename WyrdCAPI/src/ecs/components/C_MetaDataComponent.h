#pragma once

/* core wyrd includes */
#include <core/Structures.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void MetaDataComponent_SetName(void* obj, Wyrd::Entity entity, char* position);

LIBRARY_API void MetaDataComponent_SetUid(void* obj, Wyrd::Entity entity, Wyrd::UID* position);



#ifdef __cplusplus
}
#endif