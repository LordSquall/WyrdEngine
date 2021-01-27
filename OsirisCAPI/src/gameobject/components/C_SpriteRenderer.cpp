/* core osiris includes */
#include <core/scene/GameObject.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/Layer2D.h>
#include <core/renderer/Texture.h>

#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_SpriteRenderer.h" 


void* SpriteRenderer_Create(void* obj)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;

	std::shared_ptr<Osiris::SpriteComponent> spriteComponent = std::make_shared<Osiris::SpriteComponent>(std::shared_ptr<Osiris::GameObject>(gameObject));
	spriteComponent->Initialise();

	gameObject->components.push_back(spriteComponent);

	gameObject->layer->RegisterSprite(spriteComponent);

	return &*spriteComponent;
}

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

	spriteComponent->size.x = x;
	spriteComponent->size.y = y;
}

void SpriteRenderer_SetPosition(void* obj, float x, float y)
{
	Osiris::SpriteComponent* spriteComponent = (Osiris::SpriteComponent*)obj;

	spriteComponent->position.x = x;
	spriteComponent->position.y = y;
}

void SpriteRenderer_SetTexture(void* obj, void* texture)
{
	Osiris::SpriteComponent* spriteComponent = (Osiris::SpriteComponent*)obj;
	Osiris::Texture* texturePtr = (Osiris::Texture*)texture;


	Osiris::OpenGLTexture* oglTexture = (Osiris::OpenGLTexture * )texturePtr;

	spriteComponent->texture = std::move(std::shared_ptr<Osiris::Texture>(texturePtr));
}