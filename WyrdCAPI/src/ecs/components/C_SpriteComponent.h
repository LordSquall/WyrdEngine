#pragma once

/* core wyrd includes */
#include <core/Structures.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "../../C_Export.h"


#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void SpriteComponent_SetPosition(void* obj, Wyrd::Entity entity, Wyrd::Vector2* position);

LIBRARY_API void SpriteComponent_SetSize(void* obj, Wyrd::Entity entity, Wyrd::Vector2* size);

LIBRARY_API void SpriteComponent_SetColor(void* obj, Wyrd::Entity entity, Wyrd::Color* color);

#ifdef __cplusplus
}
#endif