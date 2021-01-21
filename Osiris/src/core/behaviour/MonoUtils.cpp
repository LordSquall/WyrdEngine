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
		MonoMethod* method = &*scriptComponent->GetCustomObject()->GetMethod(functionName);
		if (method != nullptr)
		{
			MonoObject* exception = nullptr;
			mono_runtime_invoke(&*scriptComponent->GetCustomObject()->GetMethod(functionName), &*scriptComponent->GetCustomObject()->Object, &args[0], &exception);

			if (exception)
			{
				mono_print_unhandled_exception(exception);
			}
		}

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
		if (method == nullptr)
		{
			// attempt to find matching method in parent
			MonoClass* parentClass = mono_class_get_parent((MonoClass*)(scriptedClass->ManagedClass));
			if (parentClass != nullptr)
			{
				MonoMethod* parentMethod = mono_class_get_method_from_name(parentClass, methodName, argumentCount);
				if (parentMethod != nullptr)
				{
					return parentMethod;
				}
				else
				{
					OSR_ERROR("Unabled to find {0} method in {1} class!", methodName, scriptedClass->GetName());
				}
			}
			else
			{
				OSR_ERROR("Unabled to find {0} method in {1} class!", methodName, scriptedClass->GetName());
			}

			if (terminateOnMissing == true)
			{
				Application::Get().Terminate();
			}
		}
		return method;
	}
}