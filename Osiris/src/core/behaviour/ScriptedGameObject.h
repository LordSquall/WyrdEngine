#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	class Behaviour;
	class GameObject;
	class ScriptedClass;

	class OSR_LIBRARY_API ScriptedGameObject
	{
	public:
		ScriptedGameObject(Behaviour* behaviour, std::shared_ptr<ScriptedClass>, std::shared_ptr<GameObject> gameObject, void* managedObject = nullptr);
		virtual ~ScriptedGameObject();

		inline const std::string& GetName() const { return _Name; }
		inline void SetName(const std::string& name) { _Name = name; }

		std::shared_ptr<GameObject> const GetGameObject();

	public:
		MonoClass* Class;
		MonoObject* Object;
		
	private:
		std::shared_ptr<GameObject> _GameObject;
		std::map<std::string, MonoMethod*> _Methods;
		std::string _Name;
	};
}