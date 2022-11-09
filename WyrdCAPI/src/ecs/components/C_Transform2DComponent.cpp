/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_Transform2DComponent.h" 

void Transform2DComponent_SetPosition(void* scenePtr, Wyrd::Entity entity, Wyrd::Vector2* position)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::Transform2DComponent* component = scene->Get<Wyrd::Transform2DComponent>(entity);

   component->position = *position;
}

void Transform2DComponent_SetRotation(void* scenePtr, Wyrd::Entity entity, float rotation)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::Transform2DComponent* component = scene->Get<Wyrd::Transform2DComponent>(entity);

   component->rotation = rotation;
}

void Transform2DComponent_SetScale(void* scenePtr, Wyrd::Entity entity, Wyrd::Vector2* scale)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::Transform2DComponent* component = scene->Get<Wyrd::Transform2DComponent>(entity);

   component->scale = *scale;
}


