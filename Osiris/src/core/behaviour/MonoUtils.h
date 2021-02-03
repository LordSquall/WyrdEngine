#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/scene/components/ScriptComponent.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	class MonoUtils
	{
	public:

		static void* ExecuteScriptMethod(ScriptComponent* scriptComponent, std::string& functionName, std::vector<void*> args);

		static MonoObject* CreateNewObject(MonoDomain* domain, std::shared_ptr<ScriptedClass> monoClass);

		static std::pair<MonoMethod*, MonoMethod*> FindPropertyInClass(std::shared_ptr<ScriptedClass> scriptedClass, const char* propertyName);

		static MonoMethod* FindMethodInClass(std::shared_ptr<ScriptedClass> scriptedClass, const char* methodName, int argumentCount, bool terminateOnMissing = false);
	};
}