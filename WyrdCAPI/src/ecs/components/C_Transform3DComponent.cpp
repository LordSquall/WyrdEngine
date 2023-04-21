/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_Transform3DComponent.h" 

void Transform3DComponent_SetPosition(void* scenePtr, Wyrd::Entity entity, Wyrd::Vector3* position)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::Transform3DComponent* component = scene->Get<Wyrd::Transform3DComponent>(entity);

   component->position = *position;
}

void Transform3DComponent_SetRotation(void* scenePtr, Wyrd::Entity entity, Wyrd::Vector3* rotation)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::Transform3DComponent* component = scene->Get<Wyrd::Transform3DComponent>(entity);

   component->rotation = *rotation;
}

void Transform3DComponent_SetScale(void* scenePtr, Wyrd::Entity entity, Wyrd::Vector3* scale)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::Transform3DComponent* component = scene->Get<Wyrd::Transform3DComponent>(entity);

   component->scale = *scale;
}


