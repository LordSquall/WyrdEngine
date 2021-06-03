#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"

/* external includes */
#include <mono/metadata/object-forward.h>

namespace Wyrd
{
	class ScriptedGameObject;

	class WYRD_LIBRARY_API ScriptedCustomObject
	{
	public:
		ScriptedCustomObject(void* domain, const ScriptedClass* scriptedClass, const ScriptedClass* scriptedEntityClass, Entity entity);
		virtual ~ScriptedCustomObject();

		inline const std::string& GetName() const { return _Name; }
		inline void SetName(const std::string& name) { _Name = name; }

		MonoMethod* GetMethod(const std::string name);

	public:
		MonoClass** Class;
		MonoObject* Object;
		std::string TypeName;
		
	private:
		std::string _Name;
		std::map<std::string, std::pair<MonoMethod*, MonoMethod*>> _Properties;
		std::map<std::string, MonoMethod*> _Methods;
	};
}