#include "C_GameObject.h" 
#include "../managers/C_Managers.h"

#include <core/scene/GameObject.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/PhysicsComponent.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/Layer2D.h>
#include <core/behaviour/Behaviour.h>

#include <core/physics/Physics.h>

const char* GameObject_Name_Get(void* obj)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	return gameObject->name.c_str();
}

int GameObject_UID_Get(void* obj)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	return (intptr_t)&gameObject->uid;
}

void GameObject_Move(void* obj, float x, float y)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	gameObject->transform2D->Translate({ x, y });
}

void GameObject_Rotate(void* obj, float angle)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	gameObject->transform2D->Rotate(angle);
}

void GameObject_Scale(void* obj, float x, float y)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	gameObject->transform2D->SetScale({ x, y });
}

void GameObject_SetVelocity(void* obj, float vX, float vY)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	
	gameObject->physics->SetVelocity({ vX, vY });
}


void GameObject_AddChild(void* obj, void* child)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	Osiris::GameObject* childGameObject = (Osiris::GameObject*)child;

	gameObject->AddChild(std::shared_ptr<GameObject>(childGameObject));
}

int GameObject_Get_Component_Count(void* obj)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	return (int)gameObject->components.size();
}

void GameObject_Get_Component(void* obj, int idx, int* type, void** componentPtr)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;

	*componentPtr = &*gameObject->components[idx];
	*type = (int)gameObject->components[idx]->GetType();
}

void* GameObject_Create(void* obj)
{
	Osiris::GameObject* parent = (Osiris::GameObject*)obj;

	std::shared_ptr<Osiris::GameObject> go = _behaviour->CreateObject<Osiris::GameObject>("NEW OBJECT");

	/* add transform 2D */
	go->transform2D = _behaviour->CreateObject<Osiris::Transform2DComponent>(go);

	parent->AddChild(go);

	return &*go;
}