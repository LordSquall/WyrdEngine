#include "C_SpriteRenderer.h" 

#include <core/scene/components/SpriteComponent.h>

#include <core/pipeline/Sprite.h>

void SpriteRenderer_SetColor(void* obj, float red, float green, float blue, float alpha)
{
	Osiris::SpriteComponent* spriteComponent = (Osiris::SpriteComponent*)obj;

	spriteComponent->color = { red, green, blue, alpha };
}

void SpriteRenderer_GetColor(void* obj, Osiris::Color** color)
{
	Osiris::SpriteComponent* spriteComponent = (Osiris::SpriteComponent*)obj;
	*color = &spriteComponent->color;
}

void SpriteRenderer_SetSize(void* obj, float x, float y)
{
	Osiris::SpriteComponent* spriteComponent = (Osiris::SpriteComponent*)obj;

	spriteComponent->sprite->SetSize((int)x, (int)y);
}

void SpriteRenderer_SetPosition(void* obj, float x, float y)
{
	Osiris::SpriteComponent* spriteComponent = (Osiris::SpriteComponent*)obj;

	spriteComponent->sprite->SetPosition((int)x, (int)y);
}