#pragma once

/* core wyrd includes */
#include <core/Structures.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void Transform3DComponent_SetPosition(void* obj, Wyrd::Entity entity, Wyrd::Vector3* position);

LIBRARY_API void Transform3DComponent_SetRotation(void* obj, Wyrd::Entity entity, Wyrd::Vector3* position);

LIBRARY_API void Transform3DComponent_SetScale(void* obj, Wyrd::Entity entity, Wyrd::Vector3* position);

LIBRARY_API void Transform3DComponent_SetModelmatrix(void* obj, Wyrd::Entity entity, glm::mat4* position);

LIBRARY_API void Transform3DComponent_SetParentmodelmatrix(void* obj, Wyrd::Entity entity, glm::mat4* position);



#ifdef __cplusplus
}
#endif