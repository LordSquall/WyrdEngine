#include "C_GameObject.h" 

#include <core/scene/GameObject.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/PhysicsComponent.h>
#include <core/scene/Layer2D.h>

#include <core/physics/Physics.h>

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

void GameObject_SetVelocity(void* obj, float vX, float vY)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	
	gameObject->physics->SetVelocity({ vX, vY });
}