#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/scene/components/ScriptComponent.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	class MonoUtils
	{
	public:

		static bool SetProperty(MonoImage* image, const std::string& ns, const std::string& cls, const std::string& prop, MonoObject* obj, std::vector<void*> value);

		static bool InvokeMethod(MonoImage* image, const std::string& ns, const std::string& cls, const std::string& method, MonoObject* obj, std::vector<void*> values);





		static void* ExecuteScriptMethod(ScriptComponent* scriptComponent, std::string& functionName, std::vector<void*> args);

		static MonoObject* CreateNewObject(MonoDomain* domain, std::shared_ptr<ScriptedClass> monoClass);

		static MonoObject* CreateNewUnmanagedObject(MonoDomain* domain, std::shared_ptr<ScriptedClass> monoClass, Behaviour* behaviour, void** unmanagedObject);

		static std::pair<MonoMethod*, MonoMethod*> FindPropertyInClass(std::shared_ptr<ScriptedClass> scriptedClass, const char* propertyName);

		static MonoMethod* FindMethodInClass(const ScriptedClass* scriptedClass, const char* methodName, int argumentCount, bool terminateOnMissing = false);
	};
}