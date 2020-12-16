#include "C_SpriteRenderer.h" 

#include <core/scene/GameObject.h>
#include <core/scene/components/SpriteComponent.h>

#include <core/pipeline/Sprite.h>

void SpriteRenderer_SetColor(void* obj, float red, float green, float blue, float alpha)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;

	gameObject->sprite->Color = { red, green, blue, alpha };
}

void SpriteRenderer_SetSize(void* obj, float x, float y)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;

	gameObject->sprite->sprite->SetSize((int)x, (int)y);
}