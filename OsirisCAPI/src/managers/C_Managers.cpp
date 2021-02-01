#include "C_Managers.h" 

#include <core/Application.h>
#include <core/scene/GameObject.h>
#include <core/scene/components/IBaseComponent.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/UID.h>

Osiris::Behaviour* _behaviour = NULL;

Osiris::Resources* _resources = NULL;

void Manager_SetBehaviour(void* behaviour)
{
	_behaviour = (Osiris::Behaviour*)behaviour;
}

void Manager_SetResources(void* resources)
{
	_resources = (Osiris::Resources*)resources;
}

void Osiris::OnPreAppCreation(Osiris::Application* app)
{

}