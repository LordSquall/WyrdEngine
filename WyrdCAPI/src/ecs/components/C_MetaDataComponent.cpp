/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_MetaDataComponent.h" 

void MetaDataComponent_SetName(void* scenePtr, Wyrd::Entity entity, char* name)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MetaDataComponent* component = scene->Get<Wyrd::MetaDataComponent>(entity);

   component->name = *name;
}

void MetaDataComponent_SetUid(void* scenePtr, Wyrd::Entity entity, Wyrd::UID* uid)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MetaDataComponent* component = scene->Get<Wyrd::MetaDataComponent>(entity);

   component->uid = *uid;
}


