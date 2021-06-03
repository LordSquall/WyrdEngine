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
	Wyrd::ECSTransform2DComponent* component = scene->Get<Wyrd::ECSTransform2DComponent>(entity);
	
	component->position.x = position->x;
	component->position.y = position->y;
}