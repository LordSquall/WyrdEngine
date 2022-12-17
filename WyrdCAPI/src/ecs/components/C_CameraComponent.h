#pragma once

/* core wyrd includes */
#include <core/Structures.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void CameraComponent_SetViewport(void* obj, Wyrd::Entity entity, Wyrd::Rect* position);

LIBRARY_API void CameraComponent_SetAspectratio(void* obj, Wyrd::Entity entity, float* position);

LIBRARY_API void CameraComponent_SetSize(void* obj, Wyrd::Entity entity, Wyrd::Vector2* position);



#ifdef __cplusplus
}
#endif