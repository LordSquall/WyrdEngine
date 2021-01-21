#pragma once

/* core osiris includes */
#include "core/Structures.h"

/* local includes */
#include "../../C_Export.h"


#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void SpriteRenderer_SetColor(void* obj, float red, float green, float blue, float alpha);

LIBRARY_API void SpriteRenderer_GetColor(void* obj, Osiris::Color** color);

LIBRARY_API void SpriteRenderer_SetSize(void* obj, float x, float y);

LIBRARY_API void SpriteRenderer_SetPosition(void* obj, float x, float y);

#ifdef __cplusplus
}
#endif