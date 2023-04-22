#pragma once

/* core wyrd includes */
#include <core/Structures.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void MeshRendererComponent_SetEnabled(void* obj, Wyrd::Entity entity, bool* position);

LIBRARY_API void MeshRendererComponent_SetColor(void* obj, Wyrd::Entity entity, Wyrd::Color* position);

LIBRARY_API void MeshRendererComponent_SetMaterial(void* obj, Wyrd::Entity entity, Wyrd::UID* position);

LIBRARY_API void MeshRendererComponent_SetModel(void* obj, Wyrd::Entity entity, Wyrd::UID* position);



#ifdef __cplusplus
}
#endif