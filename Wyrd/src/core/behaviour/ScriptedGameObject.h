#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Wyrd
{
	class Behaviour;
	class ScriptedClass;

	class WYRD_LIBRARY_API ScriptedGameObject
	{
	public:
		ScriptedGameObject(Behaviour* behaviour, std::shared_ptr<ScriptedClass>, void* managedObject = nullptr);
		virtual ~ScriptedGameObject();

		inline const std::string& GetName() const { return _Name; }
		inline void SetName(const std::string& name) { _Name = name; }

	public:
		MonoClass* Class;
		MonoObject* Object;
		
	private:
		std::map<std::string, MonoMethod*> _Methods;
		std::string _Name;

		std::vector<void*> _NativePtrMap;
	};
}