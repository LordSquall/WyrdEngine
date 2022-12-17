/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_SpriteComponent.h" 

void SpriteComponent_SetEnabled(void* scenePtr, Wyrd::Entity entity, bool enabled)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::SpriteComponent* component = scene->Get<Wyrd::SpriteComponent>(entity);

   component->enabled = enabled;
}

void SpriteComponent_SetSprite(void* scenePtr, Wyrd::Entity entity, Wyrd::UID* sprite)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::SpriteComponent* component = scene->Get<Wyrd::SpriteComponent>(entity);

   component->sprite = *sprite;
}

void SpriteComponent_SetPosition(void* scenePtr, Wyrd::Entity entity, Wyrd::Vector2* position)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::SpriteComponent* component = scene->Get<Wyrd::SpriteComponent>(entity);

   component->position = *position;
}

void SpriteComponent_SetSize(void* scenePtr, Wyrd::Entity entity, Wyrd::Vector2* size)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::SpriteComponent* component = scene->Get<Wyrd::SpriteComponent>(entity);

   component->size = *size;
}

void SpriteComponent_SetTiling(void* scenePtr, Wyrd::Entity entity, Wyrd::Vector2* tiling)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::SpriteComponent* component = scene->Get<Wyrd::SpriteComponent>(entity);

   component->tiling = *tiling;
}

void SpriteComponent_SetColor(void* scenePtr, Wyrd::Entity entity, Wyrd::Color* color)
{
   Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
   Wyrd::SpriteComponent* component = scene->Get<Wyrd::SpriteComponent>(entity);

   component->color = *color;
}


