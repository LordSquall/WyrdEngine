/* core wyrd includes */
#include <core/scene/GameObject.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/Layer2D.h>
#include <core/renderer/Texture.h>

#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_SpriteRenderer.h" 
#include "../../managers/C_Managers.h"

void* SpriteRenderer_Create(void* obj)
{
	//std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;

	//std::shared_ptr<Wyrd::SpriteComponent> spriteComponent = std::make_shared<Wyrd::SpriteComponent>(std::shared_ptr<Wyrd::GameObject>((*gameObject)));
	//spriteComponent->Initialise();
	//spriteComponent->spriteLayer = (*gameObject)->layer;

	//(*gameObject)->components.push_back(spriteComponent);
	//(*gameObject)->sprite = spriteComponent;
	//(*gameObject)->layer->RegisterSprite(spriteComponent);

	//return &*spriteComponent;
	return nullptr;
}

void SpriteRenderer_SetColor(void* obj, float red, float green, float blue, float alpha)
{
	Wyrd::SpriteComponent* spriteComponent = (Wyrd::SpriteComponent*)obj;

	spriteComponent->color = { red, green, blue, alpha };
}

void SpriteRenderer_GetColor(void* obj, Wyrd::Color** color)
{
	Wyrd::SpriteComponent* spriteComponent = (Wyrd::SpriteComponent*)obj;
	*color = &spriteComponent->color;
}

void SpriteRenderer_SetSize(void* obj, float x, float y)
{
	Wyrd::SpriteComponent* spriteComponent = (Wyrd::SpriteComponent*)obj;

	spriteComponent->SetSize(x, y);
}

void SpriteRenderer_GetSize(void* obj, Wyrd::Vector2* size)
{
	Wyrd::SpriteComponent* spriteComponent = (Wyrd::SpriteComponent*)obj;

	size->x = spriteComponent->size.x;
	size->y = spriteComponent->size.y;
}

void SpriteRenderer_SetPosition(void* obj, float x, float y)
{
	Wyrd::SpriteComponent* spriteComponent = (Wyrd::SpriteComponent*)obj;

	spriteComponent->SetPosition(x, y);
}

void SpriteRenderer_GetPosition(void* obj, Wyrd::Vector2* position)
{
	Wyrd::SpriteComponent* spriteComponent = (Wyrd::SpriteComponent*)obj;

	position->x = spriteComponent->position.x;
	position->y = spriteComponent->position.y;
}

void SpriteRenderer_SetTexture(void* obj, void* texture)
{
	Wyrd::SpriteComponent* spriteComponent = (Wyrd::SpriteComponent*)obj;
	Wyrd::Texture* texturePtr = (Wyrd::Texture*)texture;

	spriteComponent->SetTexture(_resources->Textures[texturePtr->GetUID()]);

	if (spriteComponent->GetTexture() != nullptr)
		SpriteRenderer_SetSize(obj, spriteComponent->GetTexture()->GetWidth(), spriteComponent->GetTexture()->GetHeight());
}