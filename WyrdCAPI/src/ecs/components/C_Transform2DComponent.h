#pragma once

/* core wyrd includes */
#include <core/Structures.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void Transform2DComponent_SetPosition(void* obj, Wyrd::Entity entity, Wyrd::Vector2* position);

LIBRARY_API void Transform2DComponent_SetRotation(void* obj, Wyrd::Entity entity, float* position);

LIBRARY_API void Transform2DComponent_SetScale(void* obj, Wyrd::Entity entity, Wyrd::Vector2* position);



#ifdef __cplusplus
}
#endif