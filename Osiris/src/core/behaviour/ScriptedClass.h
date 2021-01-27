#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/behaviour/PropertyDesc.h"

/* external includes */
#include <mono/jit/jit.h>

namespace Osiris
{
	class IBaseComponent;

	class OSR_LIBRARY_API ScriptedClass
	{
	public:
		ScriptedClass(const std::string& className, void* managedClass, void* domain);
		virtual ~ScriptedClass();

		inline const std::string& GetName() const { return _Name; }
		inline void SetName(const std::string& name) { _Name = name; }

		inline const std::vector<PropertyDesc> GetProperties() const { return _Properties; }

	public:
		void* Domain;
		void* ManagedClass;

	private:
		std::string _Name;

		std::vector<PropertyDesc> _Properties;
		std::map<std::string, FunctionDesc> _Functions;
	};
}