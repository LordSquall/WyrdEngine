#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/scene/GameObject.h"
#include "ScriptedClass.h"
#include "ScriptedCustomObject.h"
#include "MonoUtils.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/utils/mono-error.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Osiris
{
	ScriptedCustomObject::ScriptedCustomObject(void* domain, std::shared_ptr<ScriptedClass> scriptedClass)
	{
		/* Store the class */
		Class = scriptedClass->Class;

		/* Get a reference to the Behaviour subsystem */
		Osiris::Behaviour& behaviour = Application::Get().GetBehaviour();

		/* Map methods */
		_Properties["GameObject"]	= MonoUtils::FindPropertyInClass(scriptedClass, "GameObject");
		_Methods["OnStart"]			= MonoUtils::FindMethodInClass(scriptedClass, "OnStart", 0, false);
		_Methods["OnUpdate"]		= MonoUtils::FindMethodInClass(scriptedClass, "OnUpdate", 0, false);

		/* Create Object */
		Object = MonoUtils::CreateNewObject((MonoDomain*)domain, scriptedClass);
	}

	ScriptedCustomObject::~ScriptedCustomObject()
	{

	}

	void ScriptedCustomObject::SetGameObject(const std::shared_ptr<ScriptedGameObject> scriptedGameObject)
	{
		MonoObject* exception = nullptr;

		/* retrieve the property and getter method */
		MonoProperty* gameObjectProp = mono_class_get_property_from_name(&*Class, "GameObject");
		MonoMethod* gameObjectPropSetter = mono_property_get_set_method(gameObjectProp);

		/* build property arguments */
		void* args[1];
		args[0] = scriptedGameObject->Object;

		MonoObject* newObject = mono_runtime_invoke(_Properties["GameObject"].second, &*Object, args, &exception);

		if (exception != nullptr)
		{
			mono_print_unhandled_exception(exception);
		}
	}

	std::shared_ptr<ScriptedGameObject> const ScriptedCustomObject::GetGameObject()
	{
		return nullptr;
	}

	MonoMethod* ScriptedCustomObject::GetMethod(std::string name) 
	{
		return _Methods[name];
	}
}