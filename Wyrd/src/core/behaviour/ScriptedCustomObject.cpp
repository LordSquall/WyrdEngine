#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/scene/GameObject.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/ScriptedCustomObject.h"
#include "core/behaviour/ScriptedGameObject.h"
#include "MonoUtils.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/utils/mono-error.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Wyrd
{
	ScriptedCustomObject::ScriptedCustomObject(void* domain, std::shared_ptr<ScriptedClass> scriptedClass)
	{
		/* Store the class */
		Class = scriptedClass->ManagedClass;

		/* Store type name */
		TypeName = scriptedClass->GetName();

		/* Map methods */
		_Properties["GameObject"]		= MonoUtils::FindPropertyInClass(scriptedClass, "GameObject");
		_Methods["OnStart"]				= MonoUtils::FindMethodInClass(scriptedClass, "OnStart", 0, false);
		_Methods["OnUpdate"]			= MonoUtils::FindMethodInClass(scriptedClass, "OnUpdate", 1, false);
		_Methods["OnTriggerCollision"]	= MonoUtils::FindMethodInClass(scriptedClass, "OnTriggerCollision", 1, false);
		_Methods["OnKeyDown"]			= MonoUtils::FindMethodInClass(scriptedClass, "OnKeyDown", 1, false);
		_Methods["OnKeyPressed"]		= MonoUtils::FindMethodInClass(scriptedClass, "OnKeyPressed", 1, false);
		_Methods["OnKeyUp"]				= MonoUtils::FindMethodInClass(scriptedClass, "OnKeyUp", 1, false);

		/* Create Object */
		Object = MonoUtils::CreateNewObject((MonoDomain*)domain, scriptedClass);
	}

	ScriptedCustomObject::~ScriptedCustomObject()
	{

	}

	void ScriptedCustomObject::SetGameObject(const ScriptedGameObject& scriptedGameObject)
	{
		MonoObject* exception = nullptr;

		/* build property arguments */
		void* args[1];
		args[0] = scriptedGameObject.Object;

		MonoObject* newObject = mono_runtime_invoke(_Properties["GameObject"].second, Object, args, &exception);

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