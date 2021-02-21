#pragma once

/* core osiris includes */
#include "core/Structures.h"

/* local includes */
#include "../../C_Export.h"


#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void* SpriteRenderer_Create(void* obj);

LIBRARY_API void SpriteRenderer_SetColor(void* obj, float red, float green, float blue, float alpha);

LIBRARY_API void SpriteRenderer_GetColor(void* obj, Osiris::Color** color);

LIBRARY_API void SpriteRenderer_SetSize(void* obj, float x, float y);

LIBRARY_API void SpriteRenderer_GetSize(void* obj, Osiris::Vector2* size);

LIBRARY_API void SpriteRenderer_SetPosition(void* obj, float x, float y);

LIBRARY_API void SpriteRenderer_GetPosition(void* obj, Osiris::Vector2* position);

LIBRARY_API void SpriteRenderer_SetTexture(void* obj, void* texture);

#ifdef __cplusplus
}
#endif