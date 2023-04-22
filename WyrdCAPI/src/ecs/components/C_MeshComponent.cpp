/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_MeshRendererComponent.h" 

void MeshComponent_SetEnabled(void* scenePtr, Wyrd::Entity entity, bool enabled)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MeshComponent* component = scene->Get<Wyrd::MeshComponent>(entity);

   component->enabled = enabled;
}

void MeshComponent_SetColor(void* scenePtr, Wyrd::Entity entity, Wyrd::Color* color)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MeshComponent* component = scene->Get<Wyrd::MeshComponent>(entity);

   component->color = *color;
}


