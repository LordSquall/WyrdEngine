/* core wyrd includes */
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>
#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_TextComponent.h" 

void TextComponent_SetContent(void* scenePtr, Wyrd::Entity entity, const char* content)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	Wyrd::TextComponent* component = scene->Get<Wyrd::TextComponent>(entity);
	
	strcpy(component->content, content);
}

void TextComponent_SetSize(void* scenePtr, Wyrd::Entity entity, float size)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	Wyrd::TextComponent* component = scene->Get< Wyrd::TextComponent>(entity);

	component->size = size;
}

void TextComponent_SetColor(void* scenePtr, Wyrd::Entity entity, Wyrd::Color* color)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	Wyrd::TextComponent* component = scene->Get<Wyrd::TextComponent>(entity);

	component->color.r = color->r;
	component->color.g = color->g;
	component->color.b = color->b;
	component->color.a = color->a;
}