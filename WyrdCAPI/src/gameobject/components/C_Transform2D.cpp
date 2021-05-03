#include "C_Transform2D.h" 

#include <core/scene/components/Transform2DComponent.h>

void Transform2D_SetPosition(void* obj, float x, float y)
{
	Wyrd::Transform2DComponent* transform2D = (Wyrd::Transform2DComponent*)obj;
	transform2D->SetPosition({ x, y });
}

void Transform2D_GetPosition(void* obj, Wyrd::Vector2* vec)
{
	Wyrd::Transform2DComponent* transform2D = (Wyrd::Transform2DComponent*)obj;
	vec->x = transform2D->position.x;
	vec->y = transform2D->position.y;
}