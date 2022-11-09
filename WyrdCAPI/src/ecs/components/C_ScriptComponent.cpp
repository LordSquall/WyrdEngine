/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_ScriptComponent.h" 

void ScriptComponent_SetEnabled(void* scenePtr, Wyrd::Entity entity, bool enabled)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::ScriptComponent* component = scene->Get<Wyrd::ScriptComponent>(entity);

   component->enabled = enabled;
}

void ScriptComponent_SetScriptid(void* scenePtr, Wyrd::Entity entity, Wyrd::UID* scriptId)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::ScriptComponent* component = scene->Get<Wyrd::ScriptComponent>(entity);

   component->scriptId = *scriptId;
}

void ScriptComponent_SetInstanceid(void* scenePtr, Wyrd::Entity entity, int32_t instanceId)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::ScriptComponent* component = scene->Get<Wyrd::ScriptComponent>(entity);

   component->instanceId = instanceId;
}


