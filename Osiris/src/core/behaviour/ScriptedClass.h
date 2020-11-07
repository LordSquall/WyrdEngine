#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	class OSR_LIBRARY_API ScriptedClass
	{
	public:
		ScriptedClass(const std::string& className, void* managedClass);
		virtual ~ScriptedClass();

		inline const std::string& GetName() const { return _Name; }
		inline void SetName(const std::string& name) { _Name = name; }

	public:
		void* ManagedClass;

	private:
		std::string _Name;
	};
}