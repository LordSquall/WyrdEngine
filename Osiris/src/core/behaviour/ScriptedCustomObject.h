#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	class GameObject;
	class ScriptedGameObject;

	class OSR_LIBRARY_API ScriptedCustomObject
	{
	public:
		ScriptedCustomObject(void* domain, std::shared_ptr<ScriptedClass> scriptedClass);
		virtual ~ScriptedCustomObject();

		inline const std::string& GetName() const { return _Name; }
		inline void SetName(const std::string& name) { _Name = name; }

		void SetGameObject(const std::shared_ptr<ScriptedGameObject> scriptedGameObject);
		std::shared_ptr<ScriptedGameObject> const GetGameObject();

		MonoMethod* GetMethod(const std::string name);

	public:
		MonoClass* Class;
		MonoObject* Object;
		std::string TypeName;
		
	private:
		std::string _Name;
		std::map<std::string, std::pair<MonoMethod*, MonoMethod*>> _Properties;
		std::map<std::string, MonoMethod*> _Methods;
	};
}