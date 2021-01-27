#include "C_Managers.h" 

#include <core/Application.h>
#include <core/scene/GameObject.h>
#include <core/scene/components/IBaseComponent.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/UID.h>

Osiris::Behaviour* _behaviour = NULL;

void Manager_SetBehaviour(void* behaviour)
{
	_behaviour = (Osiris::Behaviour*)behaviour;
}

void Osiris::OnPreAppCreation(Osiris::Application* app)
{

}