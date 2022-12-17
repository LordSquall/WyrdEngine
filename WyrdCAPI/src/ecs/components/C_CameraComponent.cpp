/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_CameraComponent.h" 

void CameraComponent_SetViewport(void* scenePtr, Wyrd::Entity entity, Wyrd::Rect* viewport)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->viewport = *viewport;
}

void CameraComponent_SetAspectratio(void* scenePtr, Wyrd::Entity entity, float aspectRatio)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->aspectRatio = aspectRatio;
}

void CameraComponent_SetSize(void* scenePtr, Wyrd::Entity entity, Wyrd::Vector2* size)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->size = *size;
}


