/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_CameraComponent.h" 

void CameraComponent_SetEnabled(void* scenePtr, Wyrd::Entity entity, bool enabled)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->enabled = enabled;
}

void CameraComponent_SetScriptid(void* scenePtr, Wyrd::Entity entity, Wyrd::UID* scriptId)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->scriptId = *scriptId;
}

void CameraComponent_SetInstanceid(void* scenePtr, Wyrd::Entity entity, int32_t instanceId)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->instanceId = instanceId;
}


