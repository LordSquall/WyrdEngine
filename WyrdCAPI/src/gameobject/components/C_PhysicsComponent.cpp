/* core wyrd includes */
#include <core/scene/GameObject.h>
#include <core/scene/components/PhysicsComponent.h>
#include <core/scene/Layer2D.h>
#include <core/renderer/Texture.h>

#include <platform/OpenGL/OpenGLTexture.h>

/* local includes */
#include "C_PhysicsComponent.h" 
#include "../../managers/C_Managers.h"

void* PhysicsComponent_Create(void* obj)
{
	//std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;

	//std::shared_ptr<Wyrd::PhysicsComponent> physicsComponent = std::make_shared<Wyrd::PhysicsComponent>(std::shared_ptr<Wyrd::GameObject>((*gameObject)));
	//(*gameObject)->components.push_back(physicsComponent);

	//_physics->AddObject(physicsComponent.get());

	//return &*physicsComponent;
	return nullptr;
}

void PhysicsComponent_SetIsTrigger(void* obj, bool isTrigger)
{
	Wyrd::PhysicsComponent* physicsComponent = (Wyrd::PhysicsComponent*)obj;

	physicsComponent->SetIsTrigger(isTrigger);
}

bool PhysicsComponent_GetIsTrigger(void* obj)
{
	Wyrd::PhysicsComponent* physicsComponent = (Wyrd::PhysicsComponent*)obj;

	return physicsComponent->IsTrigger();
}

void PhysicsComponent_SetIsStatic(void* obj, bool isStatic)
{
	Wyrd::PhysicsComponent* physicsComponent = (Wyrd::PhysicsComponent*)obj;

	physicsComponent->SetIsStatic(isStatic);
}

bool PhysicsComponent_GetIsStatic(void* obj)
{
	Wyrd::PhysicsComponent* physicsComponent = (Wyrd::PhysicsComponent*)obj;

	return physicsComponent->IsStatic();
}