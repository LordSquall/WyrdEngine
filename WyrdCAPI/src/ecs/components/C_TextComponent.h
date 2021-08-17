#pragma once

/* core wyrd includes */
#include <core/Structures.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void TextComponent_SetContent(void* scenePtr, Wyrd::Entity entity, const char* content);

LIBRARY_API void TextComponent_SetSize(void* scenePtr, Wyrd::Entity entity, float size);

LIBRARY_API void TextComponent_SetColor(void* scenePtr, Wyrd::Entity entity, Wyrd::Color* color);

#ifdef __cplusplus
}
#endif