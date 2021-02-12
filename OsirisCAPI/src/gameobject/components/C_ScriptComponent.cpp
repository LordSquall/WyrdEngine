#include "C_ScriptComponent.h" 
#include "../../managers/C_Managers.h"

#include <core/Application.h>
#include <core/scene/GameObject.h>
#include <core/scene/components/ScriptComponent.h>
#include <core/behaviour/ScriptedCustomObject.h>
#include <core/behaviour/ScriptedClass.h>
#include <core/UID.h>

/* external includes */
#include <mono/jit/jit.h>
#include <mono/utils/mono-error.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

void* ScriptComponent_Create(void* obj, void* componentManagedObject)
{
	std::shared_ptr<Osiris::GameObject>* gameObject = (std::shared_ptr<Osiris::GameObject>*)obj;

	std::shared_ptr<Osiris::ScriptComponent> scriptComponent = std::make_shared<Osiris::ScriptComponent>(std::shared_ptr<Osiris::GameObject>((*gameObject)));
	scriptComponent->Initialise();
	scriptComponent->ManagedObject = mono_gchandle_get_target((uint32)componentManagedObject);

	(*gameObject)->components.push_back(scriptComponent);

	return &(*gameObject)->components.back();
}

void ScriptComponent_Enabled(void* obj, bool enable)
{

}


void ScriptComponent_CreateInstance(void* obj, void* component, const char* className)
{
	/* Cast the gameobject */
	std::shared_ptr<Osiris::GameObject>* gameObject = (std::shared_ptr<Osiris::GameObject>*)obj;

	/* Cast the component */
	std::shared_ptr<Osiris::ScriptComponent>* scriptComponent = (std::shared_ptr<Osiris::ScriptComponent>*)component;

	/* Retrieve the scripted class */
	std::shared_ptr<Osiris::ScriptedClass> scriptedClass = _behaviour->GetCustomClass(className);

	if (scriptedClass != nullptr)
	{
		/* Create the Scripted custom object */
		std::shared_ptr<Osiris::ScriptedCustomObject> newCustomObject = std::make_shared<Osiris::ScriptedCustomObject>((MonoDomain*)_behaviour->GetDomain(), scriptedClass);

		/* Store within the Behaviour Subsystem */
		_behaviour->AddScriptedCustomObject((*gameObject)->uid, newCustomObject);

		/* Set the native game object pointer */
		newCustomObject->SetGameObject(_behaviour->GetGameObject((*gameObject)->uid));

		/* Set an script name */
		newCustomObject->SetName(std::string(className) + "_inst");

		MonoProperty* instanceProperty = mono_class_get_property_from_name((MonoClass*)*_behaviour->GetClass("ScriptComponent")->ManagedClass, "Instance");
		MonoMethod* instancePropertySetter = mono_property_get_set_method(instanceProperty);

		(*scriptComponent)->SetClass(scriptedClass);
		(*scriptComponent)->SetCustomObject(newCustomObject);

		void* args[1];
		args[0] = newCustomObject->Object;

		MonoObject* exception = nullptr;
		MonoObject* newObject = mono_runtime_invoke((MonoMethod*)_behaviour->GetClass("ScriptComponent")->Properties["Instance"]->GetSetter(), (*scriptComponent)->ManagedObject, args, &exception);

		if (exception != nullptr)
		{
			mono_print_unhandled_exception(exception);
		}


		/// Checkout invoke object return :) 
	}
	else
	{
		printf("Failed to find scripted class [%s]\n", className);
	}
}