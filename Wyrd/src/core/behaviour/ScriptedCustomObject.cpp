#pragma once

/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
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
	ScriptedCustomObject::ScriptedCustomObject(void* domain, const ScriptedClass* scriptedClass, const ScriptedClass* scriptedEntityClass, Entity entity)
	{
		/* Store the class */
		Class = scriptedClass->ManagedClass;

		/* Store type name */
		TypeName = scriptedClass->GetName();

		/* Map methods */
		//_Methods["OnStart"]				= MonoUtils::FindMethodInClass(scriptedClass, "OnStart", 0, false);
		//_Methods["OnUpdate"]			= MonoUtils::FindMethodInClass(scriptedClass, "OnUpdate", 1, false);
		//_Methods["OnTriggerCollision"]	= MonoUtils::FindMethodInClass(scriptedClass, "OnTriggerCollision", 1, false);
		//_Methods["OnKeyDown"]			= MonoUtils::FindMethodInClass(scriptedClass, "OnKeyDown", 1, false);
		//_Methods["OnKeyPressed"]		= MonoUtils::FindMethodInClass(scriptedClass, "OnKeyPressed", 1, false);
		//_Methods["OnKeyUp"]				= MonoUtils::FindMethodInClass(scriptedClass, "OnKeyUp", 1, false);

		/* Create Object */
		Object = mono_object_new((MonoDomain*)domain, (MonoClass*)*scriptedClass->ManagedClass);
		if (Object == NULL)
		{
			WYRD_CORE_ERROR("mono_object_new for {0}", scriptedClass->GetName());
		}

		/* Call the object default constructor */
		mono_runtime_object_init(Object);

		MonoMethod* registerEntityMethod = mono_class_get_method_from_name((MonoClass*)*scriptedEntityClass->ManagedClass, "RegisterEntity", 1);

		MonoObject* exc = nullptr;
		void* registerEntityArgs[1] = { &entity };
		mono_runtime_invoke(registerEntityMethod, Object, registerEntityArgs, &exc);

		if (exc != nullptr) mono_print_unhandled_exception(exc);
	}

	ScriptedCustomObject::~ScriptedCustomObject()
	{

	}

	MonoMethod* ScriptedCustomObject::GetMethod(std::string name)
	{
		return _Methods[name];
	}
}