#pragma once

/* local includes */
#include "core/ecs/ECS.h"
#include "../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void SceneManager_ChangeScene(void* sceneManagerPtr, const char* sceneName);

#ifdef __cplusplus
}
#endif