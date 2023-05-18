/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_MaterialComponent.h" 

void MaterialComponent_SetEnabled(void* scenePtr, Wyrd::Entity entity, bool enabled)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);

   component->enabled = enabled;
}

void MaterialComponent_SetColor(void* scenePtr, Wyrd::Entity entity, Wyrd::Color* color)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);

   component->color = *color;
}

void MaterialComponent_SetMaterial(void* scenePtr, Wyrd::Entity entity, Wyrd::UID* material)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::MaterialComponent* component = scene->Get<Wyrd::MaterialComponent>(entity);

   component->material = *material;
}


