#include "C_GameObject.h" 

#include <core/scene/GameObject.h>

const char* GameObject_Name_Get(void* obj)
{
	Osiris::GameObject* gameObject = (Osiris::GameObject*)obj;
	return gameObject->name.c_str();
}