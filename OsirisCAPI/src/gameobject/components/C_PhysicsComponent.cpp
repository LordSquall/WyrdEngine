#include "C_PhysicsComponent.h" 

#include <core/scene/components/PhysicsComponent.h>

void PhysicsComponent_SetIsTrigger(void* obj, bool isTrigger) 
{
	Osiris::PhysicsComponent* physicsComponent = (Osiris::PhysicsComponent*)obj;

	physicsComponent->SetIsTrigger(isTrigger);
}