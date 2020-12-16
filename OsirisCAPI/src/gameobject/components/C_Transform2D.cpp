#include "C_SpriteRenderer.h" 

#include <core/scene/components/Transform2DComponent.h>

void Transform2D_SetPosition(void* obj, float x, float y)
{
	Osiris::Transform2DComponent* transform2D = (Osiris::Transform2DComponent*)obj;

	transform2D->SetPosition({ x, y });
}

void Transform2D_SetRotation(void* obj, float angle)
{
	Osiris::Transform2DComponent* transform2D = (Osiris::Transform2DComponent*)obj;

	transform2D->SetRotation(angle);
}

void Transform2D_SetScale(void* obj, float sx, float sy)
{
	Osiris::Transform2DComponent* transform2D = (Osiris::Transform2DComponent*)obj;

	transform2D->SetScale({ sx, sy });
}