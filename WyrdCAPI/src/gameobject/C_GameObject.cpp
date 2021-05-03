#include "C_GameObject.h" 
#include "../managers/C_Managers.h"

#include <core/scene/GameObject.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/PhysicsComponent.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/components/ScriptComponent.h>
#include <core/scene/Layer2D.h>
#include <core/behaviour/Behaviour.h>
#include <core/behaviour/ScriptedClass.h>

#include <core/physics/Physics.h>

#include <mono/jit/jit.h>

const char* GameObject_Name_Get(void* obj)
{
	GameObject* gameObject = (GameObject*)obj;

	return gameObject->name.c_str();
}

int GameObject_UID_Get(void* obj)
{
	GameObject* gameObject = (GameObject*)obj;
	return 0;
	///return (intptr_t)&(*gameObject)->uid;
}

void GameObject_Move(void* obj, float x, float y)
{
	GameObject* gameObject = (GameObject*)obj;
	//gameObject->transform->Translate({ x, y });
}

void GameObject_Rotate(void* obj, float angle)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	//(*gameObject)->transform2D->Rotate(angle);
}

void GameObject_Scale(void* obj, float x, float y)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	//(*gameObject)->transform2D->Scale({ x, y });
}


void GameObject_SetAngle(void* obj, float angle)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	//(*gameObject)->transform2D->SetRotation(angle);
}

void GameObject_SetVelocity(void* obj, float vX, float vY)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	
	(*gameObject)->physics->SetVelocity({ vX, vY });
}


void GameObject_AddChild(void* obj, void* child)
{
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;
	Wyrd::GameObject* childGameObject = (Wyrd::GameObject*)child;

	//(*gameObject)->AddChild(std::move(std::make_unique<GameObject>(childGameObject)));
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

void* GameObject_Create(void* parentGameObject, const char* name, bool retrieveManagedObject)
{
	std::shared_ptr<Wyrd::GameObject>* parent = (std::shared_ptr<Wyrd::GameObject>*)parentGameObject;

	//std::shared_ptr<Wyrd::GameObject> go = std::make_shared<Wyrd::GameObject>(name, true);

	//MonoObject* managedGameObject = nullptr;
	//if (retrieveManagedObject == true)
	//{
	//	MonoObject* exception = nullptr;
	//	managedGameObject = mono_runtime_invoke((MonoMethod*)_behaviour->GetClass("ObjectStore")->Properties["LastCreatedObject"]->GetGetter(), nullptr, nullptr, &exception);
	//	if (exception != nullptr) mono_print_unhandled_exception(exception);
	//}

	///* add transform 2D */
	//go->transform2D = std::make_shared<Wyrd::Transform2DComponent>(go);

	//_behaviour->AddScriptedGameObject(go->uid, go, managedGameObject);

	//std::shared_ptr<GameObject>* goAdded = (*parent)->AddChild(go);
	//(*goAdded)->parent = (*parent);

	//return goAdded;
	return nullptr;
}

void GameObject_Destroy(void* gameObject)
{
	std::shared_ptr<Wyrd::GameObject>* go = (std::shared_ptr<Wyrd::GameObject>*)gameObject;

	/* remove from physics system if applicable */
	if ((*go)->physics != nullptr)
		_physics->RemoveObject((*go)->physics.get());

}