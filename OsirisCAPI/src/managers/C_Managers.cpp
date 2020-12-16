#include "C_Managers.h" 

#include <core/scene/GameObject.h>
#include <core/scene/components/IBaseComponent.h>

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