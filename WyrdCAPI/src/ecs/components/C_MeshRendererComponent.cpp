/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_MeshRendererComponent.h" 

void MeshRendererComponent_SetEnabled(void* scenePtr, Wyrd::Entity entity, bool enabled)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MeshRendererComponent* component = scene->Get<Wyrd::MeshRendererComponent>(entity);

   component->enabled = enabled;
}

void MeshRendererComponent_SetModel(void* scenePtr, Wyrd::Entity entity, Wyrd::UID* model)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MeshRendererComponent* component = scene->Get<Wyrd::MeshRendererComponent>(entity);

   component->model = *model;
}


