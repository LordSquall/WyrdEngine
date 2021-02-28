#include "C_Managers.h" 

#include <core/Application.h>
#include <core/scene/GameObject.h>
#include <core/behaviour/ScriptedCustomObject.h>
#include <core/UID.h>

Osiris::Behaviour*	_behaviour = NULL;
Osiris::Physics*	_physics = NULL;
Osiris::Resources*	_resources = NULL;

class MonoDomain;

void Manager_SetBehaviour(void* behaviour)
{
	_behaviour = (Osiris::Behaviour*)behaviour;
}


void Manager_SetPhysics(void* physics)
{
	_physics = (Osiris::Physics*)physics;
}

void Manager_SetResources(void* resources)
{
	_resources = (Osiris::Resources*)resources;
}

void Osiris::OnPreAppCreation(Osiris::Application* app)
{

}