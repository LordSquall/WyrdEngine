#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	class MonoUtils
	{
	public:
		static MonoObject* CreateNewObject(MonoDomain* domain, std::shared_ptr<ScriptedClass> monoClass);

		static std::pair<MonoMethod*, MonoMethod*> FindPropertyInClass(std::shared_ptr<ScriptedClass> scriptedClass, const char* propertyName);

		static MonoMethod* FindMethodInClass(std::shared_ptr<ScriptedClass> scriptedClass, const char* methodName, int argumentCount, bool terminateOnMissing = false);
	};
}