#pragma once

/* core wyrd includes */
#include "core/Structures.h"

/* local includes */
#include "../../C_Export.h"


#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void SpriteComponent_SetPosition(void* obj, Entity entity, Wyrd::Vector2* position);

LIBRARY_API void SpriteComponent_SetSize(void* obj, Entity entity, Wyrd::Vector2* size);

LIBRARY_API void SpriteComponent_SetColor(void* obj, Entity entity, Wyrd::Color* color);

#ifdef __cplusplus
}
#endif