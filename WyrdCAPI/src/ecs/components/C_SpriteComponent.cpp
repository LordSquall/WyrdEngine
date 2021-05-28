/* core wyrd includes */
#include <core/scene/GameObject.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/Layer2D.h>
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>

#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_SpriteComponent.h" 


void SpriteComponent_SetPosition(void* scenePtr, Entity entity, Wyrd::Vector2* position)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	ECSSpriteComponent* component = scene->Get<ECSSpriteComponent>(entity);
	
	component->position.x = position->x;
	component->position.y = position->y;
}

void SpriteComponent_SetSize(void* scenePtr, Entity entity, Wyrd::Vector2* size)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	ECSSpriteComponent* component = scene->Get<ECSSpriteComponent>(entity);

	component->size.x = size->x;
	component->size.y = size->y;
}

void SpriteComponent_SetColor(void* scenePtr, Entity entity, Wyrd::Color* color)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	ECSSpriteComponent* component = scene->Get<ECSSpriteComponent>(entity);

	component->color.r = color->r;
	component->color.g = color->g;
	component->color.b = color->b;
	component->color.a = color->a;
}