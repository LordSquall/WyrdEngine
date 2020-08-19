#include "C_GameObject.h" 

#include <core/scene/GameObject.h>
#include <core/scene/components/Transform2DComponent.h>

const char* GameObject_Name_Get(void* obj)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	return gameObject->name.c_str();
}

void GameObject_Move(void* obj, float x, float y)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	gameObject->transform2D->Translate({ x, y });
}