#pragma once

/* core wyrd includes */
#include <core/Structures.h>
#include <core/ecs/ECS.h>

/* local includes */
#include "../../C_Export.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void MaterialComponent_SetEnabled(void* obj, Wyrd::Entity entity, bool* position);

LIBRARY_API void MaterialComponent_SetColor(void* obj, Wyrd::Entity entity, Wyrd::Color* position);

LIBRARY_API void MaterialComponent_SetMaterial(void* obj, Wyrd::Entity entity, Wyrd::UID* position);

LIBRARY_API void MaterialComponent_SetProperty_Int(void* scenePtr, Wyrd::Entity entity, char* name, int value);

LIBRARY_API void MaterialComponent_SetProperty_UInt32(void* scenePtr, Wyrd::Entity entity, char* name, uint32_t value);

LIBRARY_API void MaterialComponent_SetProperty_Int64(void* scenePtr, Wyrd::Entity entity, char* name, int64_t value);

LIBRARY_API void MaterialComponent_SetProperty_UInt64(void* scenePtr, Wyrd::Entity entity, char* name, uint64_t value);

LIBRARY_API void MaterialComponent_SetProperty_Float(void* scenePtr, Wyrd::Entity entity, char* name, float value);

LIBRARY_API void MaterialComponent_SetProperty_Double(void* scenePtr, Wyrd::Entity entity, char* name, double value);

LIBRARY_API void MaterialComponent_SetProperty_Color(void* scenePtr, Wyrd::Entity entity, char* name, Wyrd::Color* colorPtr);

LIBRARY_API void MaterialComponent_SetProperty_Texture(void* scenePtr, Wyrd::Entity entity, char* name, void* texturePtr);


#ifdef __cplusplus
}
#endif