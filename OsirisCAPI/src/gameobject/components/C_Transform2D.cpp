#include "C_Transform2D.h" 

#include <core/scene/components/Transform2DComponent.h>

void Transform2D_SetPosition(void* obj, float x, float y)
{
	Osiris::Transform2DComponent* transform2D = (Osiris::Transform2DComponent*)obj;
	transform2D->SetPosition({ x, y });
}

void Transform2D_GetPosition(void* obj, Osiris::Vector2* vec)
{
	Osiris::Transform2DComponent* transform2D = (Osiris::Transform2DComponent*)obj;
	vec->x = transform2D->position.x;
	vec->y = transform2D->position.y;
}

void Transform2D_SetRotation(void* obj, float angle)
{
	Osiris::Transform2DComponent* transform2D = (Osiris::Transform2DComponent*)obj;
	transform2D->SetRotation(angle);
}

void Transform2D_GetRotation(void* obj, float* angle)
{
	Osiris::Transform2DComponent* transform2D = (Osiris::Transform2DComponent*)obj;
	(*angle) = transform2D->rotation;
}

void Transform2D_SetScale(void* obj, float sx, float sy)
{
	Osiris::Transform2DComponent* transform2D = (Osiris::Transform2DComponent*)obj;
	transform2D->SetScale({ sx, sy });
}

void Transform2D_GetScale(void* obj, Osiris::Vector2* vec)
{
	Osiris::Transform2DComponent* transform2D = (Osiris::Transform2DComponent*)obj;
	vec->x = transform2D->scale.x;
	vec->y = transform2D->scale.y;
}