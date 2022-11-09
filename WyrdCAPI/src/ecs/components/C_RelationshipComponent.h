#pragma once

/* core wyrd includes */
#include <core/Structures.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void RelationshipComponent_SetFirst(void* obj, Wyrd::Entity entity, Wyrd::Entity* position);

LIBRARY_API void RelationshipComponent_SetPrevious(void* obj, Wyrd::Entity entity, Wyrd::Entity* position);

LIBRARY_API void RelationshipComponent_SetNext(void* obj, Wyrd::Entity entity, Wyrd::Entity* position);

LIBRARY_API void RelationshipComponent_SetParent(void* obj, Wyrd::Entity entity, Wyrd::Entity* position);

LIBRARY_API void RelationshipComponent_SetChildrencnt(void* obj, Wyrd::Entity entity, uint32_t* position);

LIBRARY_API void RelationshipComponent_SetDepth(void* obj, Wyrd::Entity entity, int32_t* position);

LIBRARY_API void RelationshipComponent_SetRemove(void* obj, Wyrd::Entity entity, bool* position);



#ifdef __cplusplus
}
#endif