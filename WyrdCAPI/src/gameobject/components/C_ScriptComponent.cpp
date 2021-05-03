#include "C_ScriptComponent.h" 
#include "../../managers/C_Managers.h"

#include <core/Application.h>
#include <core/scene/GameObject.h>
#include <core/scene/components/ScriptComponent.h>
#include <core/behaviour/ScriptedCustomObject.h>
#include <core/behaviour/ScriptedClass.h>
#include <core/behaviour/ScriptedGameObject.h>
#include <core/UID.h>

/* external includes */
#include <mono/jit/jit.h>
#include <mono/utils/mono-error.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

void* ScriptComponent_Create(void* obj)
{
	/* cast the target gameobject */
	//std::shared_ptr<Wyrd::GameObject>* gameObject = (std::shared_ptr<Wyrd::GameObject>*)obj;

	/* create a new script component */
	//std::shared_ptr<Wyrd::ScriptComponent> scriptComponent = std::make_shared<Wyrd::ScriptComponent>(std::shared_ptr<Wyrd::GameObject>((*gameObject)));
	//scriptComponent->Initialise();

	/* retrieve the matching managed object */
	//MonoObject* exception = nullptr;
	//scriptComponent->ManagedObject = mono_runtime_invoke((MonoMethod*)_behaviour->GetClass("ObjectStore")->Properties["LastCreatedObject"]->GetGetter(), nullptr, nullptr, &exception);
	//if (exception != nullptr) mono_print_unhandled_exception(exception);

	//(*gameObject)->components.push_back(scriptComponent);

	//return &(*gameObject)->components.back();
	return nullptr;
}

void ScriptComponent_Enabled(void* obj, bool enable)
{

}


void ScriptComponent_CreateInstance(void* obj, void* component, const char* className)
{
	/* Cast the gameobject */
	std::shared_ptr<Wyrd::GameObject>* gameObject = (std::shared_ptr<Wyrd::GameObject>*)obj;

	/* Cast the component */
	std::shared_ptr<Wyrd::ScriptComponent>* scriptComponent = (std::shared_ptr<Wyrd::ScriptComponent>*)component;

	/* Retrieve the scripted class */
	std::shared_ptr<Wyrd::ScriptedClass> scriptedClass = _behaviour->GetCustomClass(className);

	if (scriptedClass != nullptr)
	{
		/* Create a new uuid for the custom object */
		Wyrd::UID uid = Wyrd::UIDUtils::Create();

		/* Create the Scripted custom object */
		_behaviour->AddScriptedCustomObject(uid, std::make_shared<Wyrd::ScriptedCustomObject>((MonoDomain*)_behaviour->GetDomain(), scriptedClass));

		/* Create the Scripted custom object */
		auto newCustomObject = _behaviour->GetCustomObject(uid);

		/* Set the scripted game object on the new object */
		newCustomObject->SetGameObject(*(_behaviour->GetGameObject((*gameObject)->uid)));

		/* Set an script name */
		newCustomObject->SetName(std::string(className) + "_inst");

		/* Set the class and custom object of the script component*/
		(*scriptComponent)->SetClass(scriptedClass);
		(*scriptComponent)->SetCustomObject(newCustomObject);

		void* args[1];
		args[0] = newCustomObject->Object;

		MonoObject* exception = nullptr;
		MonoObject* newObject = mono_runtime_invoke((MonoMethod*)_behaviour->GetClass("ScriptComponent")->Properties["Instance"]->GetSetter(), (*scriptComponent)->ManagedObject, args, &exception);
		if (exception != nullptr) mono_print_unhandled_exception(exception);

		/* fire the OnStart function */
		mono_runtime_invoke((MonoMethod*)scriptedClass->Methods["OnStart"]->GetManagedMethod(), newCustomObject->Object, nullptr, &exception);
		if (exception != nullptr) mono_print_unhandled_exception(exception);
	}
	else
	{
		printf("Failed to find scripted class [%s]\n", className);
	}
}