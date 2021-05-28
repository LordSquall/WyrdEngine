#pragma once

/* Local includes */
#include "wyrdpch.h"
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

namespace Wyrd
{
	bool MonoUtils::SetProperty(MonoImage* image, const std::string& ns, const std::string& cls, const std::string& prop, MonoObject* obj, std::vector<void*> value)
	{
		MonoObject* exc = nullptr;

		/* retrieve the class */
		MonoClass* clsPtr = mono_class_from_name(image, ns.c_str(), cls.c_str());
		if (clsPtr == nullptr)
		{
			WYRD_ERROR("class not found: {0}", ns + "::" + cls);
			return false;
		}

		/* retrieve the property */
		MonoProperty* propPtr = mono_class_get_property_from_name(clsPtr, prop.c_str());
		if (propPtr == nullptr)
		{
			WYRD_ERROR("property not found: {0}", ns + "::" + cls + "::" + prop);
			return false;
		}

		mono_property_set_value(propPtr, obj, &value[0], &exc);
		if (exc != nullptr)
		{
			mono_print_unhandled_exception(exc);
			return false;
		}

		return true;
	}

	bool MonoUtils::InvokeMethod(MonoImage* image, const std::string& ns, const std::string& cls, const std::string& method, MonoObject* obj, std::vector<void*> value)
	{
		MonoObject* exc = nullptr;

		/* retrieve the class */
		MonoClass* clsPtr = mono_class_from_name(image, ns.c_str(), cls.c_str());
		if (clsPtr == nullptr)
		{
			WYRD_ERROR("class not found: {0}", ns + "::" + cls);
			return false;
		}

		/* retrieve the property */
		MonoMethod* methodPtr = mono_class_get_method_from_name(clsPtr, method.c_str(), value.size());
		if (methodPtr == nullptr)
		{
			WYRD_ERROR("method not found: {0}", ns + "::" + cls + "::" + method);
			return false;
		}

		mono_runtime_invoke(methodPtr, obj, (value.size() == 0) ? nullptr : &value[0], &exc);
		if (exc != nullptr)
		{
			mono_print_unhandled_exception(exc);
			return false;
		}

		return true;
	}


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
		MonoObject* object = mono_object_new((MonoDomain*)domain, (MonoClass*)*monoClass->ManagedClass);
		if (object == NULL)
		{
			WYRD_CORE_ERROR("mono_object_new for {0}", monoClass->GetName());
		}

		/* Call the object default constructor */
		mono_runtime_object_init(object);

		return object;
	}

	MonoObject* MonoUtils::CreateNewUnmanagedObject(MonoDomain* domain, std::shared_ptr<ScriptedClass> monoClass, Behaviour* behaviour, void** unmanagedObject)
	{
		MonoObject* object = mono_object_new((MonoDomain*)domain, (MonoClass*)*monoClass->ManagedClass);
		if (object == NULL)
		{
			WYRD_CORE_ERROR("mono_object_new for {0}", monoClass->GetName());
		}

		/* Call the object default constructor */
		mono_runtime_object_init(object);

		/* */
		MonoObject* exception = nullptr;
		void* nativePtrArgs[1] = { unmanagedObject };
		mono_runtime_invoke((MonoMethod*)behaviour->GetClass("UnmanagedObject")->Properties["NativePtr"]->GetSetter(), object, &nativePtrArgs[0], &exception);
		if (exception != nullptr) mono_print_unhandled_exception(exception);

		return object;
	}

	
	std::pair<MonoMethod*, MonoMethod*> MonoUtils::FindPropertyInClass(std::shared_ptr<ScriptedClass> scriptedClass, const char* propertyName)
	{
		MonoProperty* gameObjectProp = mono_class_get_property_from_name((MonoClass*)*scriptedClass->ManagedClass, propertyName);

		return { mono_property_get_get_method(gameObjectProp), mono_property_get_set_method(gameObjectProp) };
	}

	MonoMethod* MonoUtils::FindMethodInClass(const ScriptedClass* scriptedClass, const char* methodName, int argumentCount, bool terminateOnMissing)
	{
		MonoMethod* method = mono_class_get_method_from_name((MonoClass*)*scriptedClass->ManagedClass, methodName, argumentCount);
		if (method == nullptr)
		{
			// attempt to find matching method in parent
			MonoClass* parentClass = mono_class_get_parent((MonoClass*)*scriptedClass->ManagedClass);
			if (parentClass != nullptr)
			{
				MonoMethod* parentMethod = mono_class_get_method_from_name(parentClass, methodName, argumentCount);
				if (parentMethod != nullptr)
				{
					return parentMethod;
				}
				else
				{
					WYRD_ERROR("Unabled to find {0} method in {1} class!", methodName, scriptedClass->GetName());
				}
			}
			else
			{
				WYRD_ERROR("Unabled to find {0} method in {1} class!", methodName, scriptedClass->GetName());
			}

			if (terminateOnMissing == true)
			{
				Application::Get().Terminate();
			}
		}
		return method;
	}
}