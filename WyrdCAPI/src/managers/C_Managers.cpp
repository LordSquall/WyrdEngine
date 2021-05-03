#include "C_Managers.h" 

#include <core/Application.h>
#include <core/scene/GameObject.h>
#include <core/behaviour/ScriptedCustomObject.h>
#include <core/UID.h>

Wyrd::Behaviour*	_behaviour = NULL;
Wyrd::Physics*	_physics = NULL;
Wyrd::Resources*	_resources = NULL;

class MonoDomain;

void Manager_SetBehaviour(void* behaviour)
{
	_behaviour = (Wyrd::Behaviour*)behaviour;
}


void Manager_SetPhysics(void* physics)
{
	_physics = (Wyrd::Physics*)physics;
}

void Manager_SetResources(void* resources)
{
	_resources = (Wyrd::Resources*)resources;
}

void Wyrd::OnPreAppCreation(Wyrd::Application* app)
{

}