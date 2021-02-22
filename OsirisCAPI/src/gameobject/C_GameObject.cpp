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
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;

	return (*gameObject)->name.c_str();
}

int GameObject_UID_Get(void* obj)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	return (intptr_t)&(*gameObject)->uid;
}

void GameObject_Move(void* obj, float x, float y)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	(*gameObject)->transform2D->Translate({ x, y });
}

void GameObject_Rotate(void* obj, float angle)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	(*gameObject)->transform2D->Rotate(angle);
}

void GameObject_Scale(void* obj, float x, float y)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	(*gameObject)->transform2D->Scale({ x, y });
}


void GameObject_SetAngle(void* obj, float angle)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	(*gameObject)->transform2D->SetRotation(angle);
}

void GameObject_SetVelocity(void* obj, float vX, float vY)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	
	(*gameObject)->physics->SetVelocity({ vX, vY });
}


void GameObject_AddChild(void* obj, void* child)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	Osiris::GameObject* childGameObject = (Osiris::GameObject*)child;

	(*gameObject)->AddChild(std::shared_ptr<GameObject>(childGameObject));
}

int GameObject_Get_Component_Count(void* obj)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	return (int)(*gameObject)->components.size();
}

void GameObject_Get_Component(void* obj, int idx, int* type, void** componentPtr)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;

	*componentPtr = &*(*gameObject)->components[idx];
	*type = (int)(*gameObject)->components[idx]->GetType();
}

void* GameObject_Create(void* parentGameObject, const char* name)
{
	std::shared_ptr<Osiris::GameObject>* parent = (std::shared_ptr<Osiris::GameObject>*)parentGameObject;

	std::shared_ptr<Osiris::GameObject> go = std::make_shared<Osiris::GameObject>(name, true);

	/* add transform 2D */
	go->transform2D = std::make_shared<Osiris::Transform2DComponent>(go);

	_behaviour->AddScriptedGameObject(go->uid, go);

	std::shared_ptr<GameObject>* goAdded = (*parent)->AddChild(go);
	(*goAdded)->parent = (*parent);

	return goAdded;
}