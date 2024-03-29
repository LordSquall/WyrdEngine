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

void CameraComponent_SetProjection(void* scenePtr, Wyrd::Entity entity, int32_t projection)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->projection = projection;
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

void CameraComponent_SetNearplane(void* scenePtr, Wyrd::Entity entity, float nearPlane)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->nearPlane = nearPlane;
}

void CameraComponent_SetFarplane(void* scenePtr, Wyrd::Entity entity, float farPlane)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->farPlane = farPlane;
}

void CameraComponent_SetAspect(void* scenePtr, Wyrd::Entity entity, float aspect)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->aspect = aspect;
}

void CameraComponent_SetFov(void* scenePtr, Wyrd::Entity entity, float fov)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->fov = fov;
}

void CameraComponent_SetTop(void* scenePtr, Wyrd::Entity entity, float top)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->top = top;
}

void CameraComponent_SetBottom(void* scenePtr, Wyrd::Entity entity, float bottom)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->bottom = bottom;
}

void CameraComponent_SetLeft(void* scenePtr, Wyrd::Entity entity, float left)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->left = left;
}

void CameraComponent_SetRight(void* scenePtr, Wyrd::Entity entity, float right)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::CameraComponent* component = scene->Get<Wyrd::CameraComponent>(entity);

   component->right = right;
}


