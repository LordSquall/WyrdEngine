#pragma once

/* Local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/ScriptComponent.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/ScriptedGameObject.h"
#include "core/behaviour/ScriptedCustomObject.h"
#include "MonoUtils.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Osiris
{
	void* MonoUtils::ExecuteScriptMethod(ScriptComponent* scriptComponent, std::string& functionName, std::vector<void*> args)
	{
		mono_runtime_invoke(&*scriptComponent->GetCustomObject()->GetMethod(functionName), &*scriptComponent->GetCustomObject()->Object, &args[0], nullptr);

		return nullptr;
	}


	MonoObject* MonoUtils::CreateNewObject(MonoDomain* domain, std::shared_ptr<ScriptedClass> monoClass)
	{
		MonoObject* object = mono_object_new((MonoDomain*)domain, (MonoClass*)(monoClass->ManagedClass));
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
		MonoProperty* gameObjectProp = mono_class_get_property_from_name((MonoClass*)scriptedClass->ManagedClass, propertyName);

		return { mono_property_get_get_method(gameObjectProp), mono_property_get_set_method(gameObjectProp) };
	}

	MonoMethod* MonoUtils::FindMethodInClass(std::shared_ptr<ScriptedClass> scriptedClass, const char* methodName, int argumentCount, bool terminateOnMissing)
	{
		MonoMethod* method = mono_class_get_method_from_name((MonoClass*)(scriptedClass->ManagedClass), methodName, argumentCount);
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