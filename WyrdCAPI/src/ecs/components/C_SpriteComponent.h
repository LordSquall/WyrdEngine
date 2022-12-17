#pragma once

/* core wyrd includes */
#include <core/Structures.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void SpriteComponent_SetEnabled(void* obj, Wyrd::Entity entity, bool* position);

LIBRARY_API void SpriteComponent_SetSprite(void* obj, Wyrd::Entity entity, Wyrd::UID* position);

LIBRARY_API void SpriteComponent_SetPosition(void* obj, Wyrd::Entity entity, Wyrd::Vector2* position);

LIBRARY_API void SpriteComponent_SetSize(void* obj, Wyrd::Entity entity, Wyrd::Vector2* position);

LIBRARY_API void SpriteComponent_SetTiling(void* obj, Wyrd::Entity entity, Wyrd::Vector2* position);

LIBRARY_API void SpriteComponent_SetColor(void* obj, Wyrd::Entity entity, Wyrd::Color* position);



#ifdef __cplusplus
}
#endif