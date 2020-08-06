#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/scene/GameObject.h"
#include "ScriptedClass.h"
#include "ScriptedGameObject.h"
#include "MonoUtils.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Osiris
{
	MonoObject* MonoUtils::CreateNewObject(MonoDomain* domain, std::shared_ptr<ScriptedClass> monoClass)
	{
		MonoObject* object = mono_object_new((MonoDomain*)domain, &*monoClass->Class);
		if (object == NULL)
		{
			OSR_CORE_ERROR("mono_object_new for {0}", monoClass->GetName());
		}

		/* Call the object default constructor */
		mono_runtime_object_init(object);

		return object;
	}

	
	std::pair<MonoMethod*, MonoMethod*> MonoUtils::FindPropertyInClass(std::shared_ptr<ScriptedClass> scriptedClass, const char* propertyName)
	{
		MonoProperty* gameObjectProp = mono_class_get_property_from_name(scriptedClass->Class, propertyName);

		return { mono_property_get_get_method(gameObjectProp), mono_property_get_set_method(gameObjectProp) };
	}

	MonoMethod* MonoUtils::FindMethodInClass(std::shared_ptr<ScriptedClass> scriptedClass, const char* methodName, int argumentCount, bool terminateOnMissing)
	{
		MonoMethod* method = mono_class_get_method_from_name(&*(scriptedClass->Class), methodName, argumentCount);
		if (method == NULL)
		{
			OSR_ERROR("Unabled to find {0} method in {1} class!", methodName, scriptedClass->GetName());

			if (terminateOnMissing == true)
			{
				Application::Get().Terminate();
			}
		}
		return method;
	}
}