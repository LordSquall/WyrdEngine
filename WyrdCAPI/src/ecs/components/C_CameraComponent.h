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

LIBRARY_API void CameraComponent_SetProjection(void* obj, Wyrd::Entity entity, int32_t* position);

LIBRARY_API void CameraComponent_SetAspectratio(void* obj, Wyrd::Entity entity, float* position);

LIBRARY_API void CameraComponent_SetSize(void* obj, Wyrd::Entity entity, Wyrd::Vector2* position);

LIBRARY_API void CameraComponent_SetNearplane(void* obj, Wyrd::Entity entity, float* position);

LIBRARY_API void CameraComponent_SetFarplane(void* obj, Wyrd::Entity entity, float* position);

LIBRARY_API void CameraComponent_SetAspect(void* obj, Wyrd::Entity entity, float* position);

LIBRARY_API void CameraComponent_SetFov(void* obj, Wyrd::Entity entity, float* position);

LIBRARY_API void CameraComponent_SetTop(void* obj, Wyrd::Entity entity, float* position);

LIBRARY_API void CameraComponent_SetBottom(void* obj, Wyrd::Entity entity, float* position);

LIBRARY_API void CameraComponent_SetLeft(void* obj, Wyrd::Entity entity, float* position);

LIBRARY_API void CameraComponent_SetRight(void* obj, Wyrd::Entity entity, float* position);



#ifdef __cplusplus
}
#endif