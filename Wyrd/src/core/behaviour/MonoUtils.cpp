/* Local includes */
#include "wyrdpch.h"
#include "core/Log.h"
#include "core/Application.h"
#include "core/behaviour/ScriptedClass.h"
#include "core/behaviour/ScriptedCustomObject.h"
#include "MonoUtils.h"

/* external includes */
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#define WYRD_CORE_API_NAMESPACE "WyrdAPI"
#define WYRD_GAME_API_NAMESPACE "WyrdGame"

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

		/* retrieve the method */
		MonoMethod* methodPtr = mono_class_get_method_from_name(clsPtr, method.c_str(), (int)value.size());
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

	MonoMethod* MonoUtils::FindMethodInClass(MonoImage* image, const std::string& ns, const std::string& cls, const std::string& method, int argumentCount)
	{
		/* retrieve the class */
		MonoClass* clsPtr = mono_class_from_name(image, ns.c_str(), cls.c_str());
		if (clsPtr == nullptr)
		{
			WYRD_ERROR("class not found: {0}", ns + "::" + cls);
			return nullptr;
		}

		/* retrieve the method */
		MonoMethod* methodPtr = mono_class_get_method_from_name(clsPtr, method.c_str(), argumentCount);
		if (methodPtr == nullptr)
		{
			WYRD_ERROR("method not found: {0}", ns + "::" + cls + "::" + method);
			return nullptr;
		}

		return methodPtr;
	}
}