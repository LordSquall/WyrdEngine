#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "core/behaviour/Properties/ScriptProperty.h"

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

		inline const std::vector<std::shared_ptr<ScriptProperty>>& GetProperties() const { return _Properties; }

		std::unique_ptr<std::vector<std::shared_ptr<ScriptProperty>>> GetPropertiesCopy() const;

	public:
		void* Domain;
		void* ManagedClass;

	private:
		std::string _Name;

		std::vector<std::shared_ptr<ScriptProperty>> _Properties;
	};
}