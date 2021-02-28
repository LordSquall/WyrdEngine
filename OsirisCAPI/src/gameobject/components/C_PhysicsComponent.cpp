/* core osiris includes */
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
	std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)obj;

	std::shared_ptr<Osiris::PhysicsComponent> physicsComponent = std::make_shared<Osiris::PhysicsComponent>(std::shared_ptr<Osiris::GameObject>((*gameObject)));
	(*gameObject)->components.push_back(physicsComponent);

	_physics->AddObject(physicsComponent.get());

	return &*physicsComponent;
}

void PhysicsComponent_SetIsTrigger(void* obj, bool isTrigger)
{
	Osiris::PhysicsComponent* physicsComponent = (Osiris::PhysicsComponent*)obj;

	physicsComponent->SetIsTrigger(isTrigger);
}

bool PhysicsComponent_GetIsTrigger(void* obj)
{
	Osiris::PhysicsComponent* physicsComponent = (Osiris::PhysicsComponent*)obj;

	return physicsComponent->IsTrigger();
}

void PhysicsComponent_SetIsStatic(void* obj, bool isStatic)
{
	Osiris::PhysicsComponent* physicsComponent = (Osiris::PhysicsComponent*)obj;

	physicsComponent->SetIsStatic(isStatic);
}

bool PhysicsComponent_GetIsStatic(void* obj)
{
	Osiris::PhysicsComponent* physicsComponent = (Osiris::PhysicsComponent*)obj;

	return physicsComponent->IsStatic();
}