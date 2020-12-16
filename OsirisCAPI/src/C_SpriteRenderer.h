#ifndef _C_SCRIPTRENDERER_H_
#define _C_SCRIPTRENDERER_H_

/* local includes */
#include "C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void SpriteRenderer_SetColor(void* obj, float red, float green, float blue, float alpha);

LIBRARY_API void SpriteRenderer_SetSize(void* obj, float x, float y);

#ifdef __cplusplus
}
#endif

#endif