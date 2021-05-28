/* core wyrd includes */
#include <core/scene/GameObject.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/Layer2D.h>
#include <core/renderer/Texture.h>
#include <core/scene/Scene.h>
#include <core/ecs/Components.h>

#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_Transform2DComponent.h" 


void Transform2DComponent_SetPosition(void* scenePtr, Entity entity, Wyrd::Vector2* position)
{
	Wyrd::Scene* scene = (Wyrd::Scene*)scenePtr;
	ECSTransform2DComponent* component = scene->Get<ECSTransform2DComponent>(entity);
	
	component->position.x = position->x;
	component->position.y = position->y;
}